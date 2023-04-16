#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <bits/stdc++.h>

#include <vector>
#include <algorithm>
#include <random>
#include <limits>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::seconds;

//Definimos structs para manipular localizaciones (x,y).
struct location{
    long double x;
    long double y;
};

struct customer_location{
    location l;
    long double mass;
};

//Definimos una operación como:
// - un punto start (igual para el dron y camión)
// - un punto end (igual para el dron y camion
// - Un vector de pares (dron, cliente)
struct operation{
    location start;
    location end;
    vector<pair<int, customer_location>> clients_attended;
};

vector<location> GenerateRandomInitialSolution(vector<location> V_nodes);
long double distance(location a, location b);
void create_path_file(vector<location> V_nodes);
vector<customer_location> getNearbyClients(vector<customer_location> C_nodes, location l, long double range);
vector<operation> getOperations(vector<location> V_nodes, vector<customer_location> &C_nodes, int k, int capacity, float truck_speed, float drone_speed);
operation AssignDronesToClients(location V_start, location V_end, int k, int capacity, vector<customer_location> nearby_clients, vector<customer_location> &C_nodes);
void deleteClient(vector<customer_location> &C_nodes, long double x, long double y);
long double eval(vector<operation> operations,vector<customer_location> C_nodes, float truck_speed, float drone_speed, int k);
vector<vector<location>> getNeighbours(vector<location> V_nodes);
void simple_swap(vector<location> &vec, int i, int j);
void drawOperations(vector<operation> operations, int k);
void printResults(vector<operation> solution, vector<customer_location> C_nodes, float truck_speed, float drone_speed, int k);
void deleteVNode(vector<location> &V_nodes, long double x, long double y);
vector<location> GenerateGreedyInitialSolution(vector<location> V_nodes);
vector<operation> removeUnnecessaryTrips(vector<operation> operations);

int main(int argc, char* argv[]){
    //--------------------- Lectura del Archivo -----------------------
    string file = "MVDRP_Instances/";
    string file_name = "";
    file_name.append(argv[1]);
    file_name.append(" ");
    file_name.append(argv[2]);
    file.append(file_name);
    ifstream archivo(file);
    cout.precision(20);
    
    //Creamos las variables que usaremos para almacenar los datos
    int V;
    int C;
    float truck_speed;
    float drone_speed;
    location depot_location;
    vector<location> V_nodes; 
    vector<customer_location> C_nodes; 

    //Leemos el archivo, procesando los strings cuando es necesario.
    string aux;
    archivo >> aux;
    aux.erase(0,4);
    V = stoi(aux); //Almacenamos V

    archivo >> aux;
    aux.erase(0,4);
    C = stoi(aux); //Almacenamos C
    
    //Almacenamos la velocidad del camion
    for (int i = 0; i < 3; i++) archivo >> aux;
    truck_speed = stof(aux);

    //Almacenamos la velocidad del dron
    for (int i = 0; i < 3; i++) archivo >> aux;
    drone_speed = stof(aux);
    for (int i = 0; i < 4; i++) archivo >> aux;

    long double x_aux;
    long double y_aux;
    x_aux = stold(aux);
    archivo >> aux;
    y_aux = stold(aux);
    depot_location = {x_aux,y_aux}; //Almacenamos la posicion del depot

    for (int i = 0; i < 7; i++) archivo >> aux;

    //Almacenamos todas las posiciones de los nodos en V
    int i = 0;
    while(aux.compare("customer") != 0){
        x_aux = stold(aux);
        archivo >> aux;
        y_aux = stold(aux);
        archivo >> aux;
        V_nodes.push_back({x_aux,y_aux});
    }

    location start_node = V_nodes[0];
    for (int i = 0; i < 8; i++) archivo >> aux;

    //Almacenamos todas las posiciones de los nodos en C
    i = 0;
    long double mass_aux;
    while(archivo >> aux){
        x_aux = stold(aux);
        archivo >> aux;
        y_aux = stold(aux);
        archivo >> aux;
        mass_aux = stold(aux);
        C_nodes.push_back({{x_aux,y_aux},mass_aux});
    }

    //-----------------Comenzamos el algoritmo--------------------
    auto start_time = chrono::high_resolution_clock::now();

    int k = stoi(argv[3]);
    int capacity = stoi(argv[4]);
    int restarts = stoi(argv[5]);

    
    vector<operation> final_solution;
    
    long double final_eval = numeric_limits<long double>::infinity();
    for (int restart = 0; restart < restarts; restart++){

        //Al hacer restart, reiniciamos las variables del problema
        vector<customer_location> aux_C_nodes = C_nodes;  

        vector<location> TSP = GenerateRandomInitialSolution(V_nodes); 
        if (restart == restarts-1) TSP = GenerateGreedyInitialSolution(V_nodes); //Generamos un TSP Greedy solo en el ultimo restart
        
        vector<operation> current_solution = getOperations(TSP, aux_C_nodes, k, capacity, truck_speed, drone_speed);//Obtenemos las operaciones
        long double current_eval = eval(current_solution,C_nodes, truck_speed, drone_speed, k); //Evaluamos la solución

        

        bool flag_improving = true;
        //Definimos la mejor solución y evaluación para comparar en Hill-Climbing
        while(flag_improving){
            //Generamos soluciones vecinas
            vector<vector<location>> neighbours = getNeighbours(TSP);
            //Recorremos los vecinos, buscando el mejor
            flag_improving = false;
            for (vector<location> neighbour : neighbours){
                aux_C_nodes = C_nodes;

                //Para cada solucion vecina, reasignamos las operaciones (optimizable)
                vector<operation> new_solution = getOperations(neighbour, aux_C_nodes,k, capacity, truck_speed, drone_speed);
                long double new_eval = eval(new_solution, C_nodes, truck_speed, drone_speed, k); //Evaluamos al vecino

                //Recordamos solo al mejor de los vecinos, y solo si es mejor que la solución actual.
                if (new_eval < current_eval){
                    current_eval = new_eval;
                    current_solution = new_solution;                                                                                                                      
                    TSP = neighbour;
                    flag_improving = true; //Al encontrar un vecino mejor que la solucion actual, iteraremos, creando un nuevo vecindario a partir del vecino.
                }  
            } 
        }
        if(current_eval < final_eval){
            final_solution = current_solution;
            final_eval = current_eval;
        } 


    }
    final_solution = removeUnnecessaryTrips(final_solution);
    final_eval = eval(final_solution,C_nodes,truck_speed,drone_speed,k);
    
    
    auto end_time = chrono::high_resolution_clock::now();

    auto total_time= duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
   

    //-----------------Creación de archivo output.txt-------------------- 
    ofstream output("output.txt");
    int len;
    if (file_name.length() == 14) len = 10;
    else len = 11;
    output << file_name.substr(0,len) << ", k = " << k << ", capacidad dron = " << capacity << "kg" << ", tiempo de ejecución = ";
    output << total_time << "s, " <<  "valor objetivo = " << final_eval << "s" << endl;
    for(operation o : final_solution){
        output << "Camion viajó desde (" << o.start.x << "," << o.start.y << ") a (" << o.end.x << "," << o.end.y << ")" << endl;
        for (pair<int,customer_location> p : o.clients_attended){
            output << "dron " << p.first + 1 << " atendió a cliente = (" << p.second.l.x << "," << p.second.l.y << ")" << endl;
        }
    }
    
    
    output.close();
    drawOperations(final_solution, k); //Esta función genera path.txt, que luego puede ser dibujado por Path_Visualizer.py
    return 0;
}

//-------------------Fin de main-----------------------



void simple_swap(vector<location> &vec, int i, int j){
    location aux;
    aux = vec.at(i);
    vec.at(i) = vec.at(j);
    vec.at(j) = aux;
}


//Método de generación de vecinos para Hill-Climbing mejor mejora
vector<vector<location>> getNeighbours(vector<location> V_nodes){
    vector<vector<location>> neighbours;
    vector<location> neighbour = V_nodes;
    location aux;
    int n = V_nodes.size();
    int next;
    for(int i = 1; i < n-1; i++){
        simple_swap(neighbour,i,(i+1)%n); //Hacemos swap
        neighbours.push_back(neighbour); //Almacenamos el vecino en el vector de vecinos
        simple_swap(neighbour,i,(i+1)%n); //Hacemos un swap devuelta
    }
    //El ultimo nodo es un caso particular pues no puede hacer swap con el primero: Debe hacerlo con el segundo.
    simple_swap(neighbour,n-1,1); 
    neighbours.push_back(neighbour); 
    simple_swap(neighbour,n-1,1); 

    return neighbours;
}

//Creamos una función para generar la solución aleatoria inicial
vector<location> GenerateRandomInitialSolution(vector<location> V_nodes){
    //Generamos un aleatorizador para la generación de soluciones iniciales
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    auto rng = default_random_engine{seed = seed};
    //Aleatorizamos V_nodes, dejando el nodo inicial en su lugar.
    shuffle(next(begin(V_nodes),1), end(V_nodes), rng);

    return V_nodes;
};

//Creamos una función para generar la solución greedy inicial
vector<location> GenerateGreedyInitialSolution(vector<location> V_nodes){
    vector<location> greedySolution;
    greedySolution.push_back(V_nodes[0]); //El primer nodo se mantiene igual
    deleteVNode(V_nodes, V_nodes[0].x, V_nodes[0].y);
    int n = V_nodes.size();
    int i = 0;
    location closest_location;
    long double shortest_distance;
    while(i < n){
        shortest_distance = numeric_limits<long double>::infinity();
        for (location l : V_nodes){
            long double d = distance(greedySolution[i],l);
            if (d < shortest_distance){
                closest_location = l;
                shortest_distance = d;
            }
        }
        greedySolution.push_back(closest_location); //Agregamos el más cercano a la solución
        deleteVNode(V_nodes, closest_location.x, closest_location.y); //Eliminamos el nodo para no considerarlo a futuro
        i++;
    }
    return greedySolution;
    
};

//Definimos una función para calcular la distancia euclidiana entre dos localizaciones
long double distance(location a, location b){
    return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}

//Definimos la función de evaluación
long double eval(vector<operation> operations, vector<customer_location> C_nodes, float truck_speed, float drone_speed, int k){
    //El costo de un tramo será el máximo entre el costo del viaje del camión y de los drones.
    long double truck_cost = 0;
    long double max_drone_cost;

    vector<customer_location> clients_in_solution;
    
    long double total_cost = 0;
    for(operation o : operations){
        max_drone_cost = 0;
        //Subrutina para encontrar el viaje de costo máximo entre los drones
        for(int i = 0; i < k; i++){ 
            location position = o.start;
            long double drone_cost = 0; //El viaje de cada dron comienza en la posición inicial de la operación
            for (pair<int,customer_location> p : o.clients_attended){
                if (p.first == i){
                    drone_cost += distance(position, p.second.l); //Sumamos la distancia recorrida desde su posición actual hasta el nuevo cliente
                    position = p.second.l;//Actualizamos la posición del dron 
                    clients_in_solution.push_back(p.second);
                }
            }
            drone_cost += distance(position, o.end); //Agregamos el tramo final del dron para llegar al nodo final de la operación.
            if (drone_cost > max_drone_cost) max_drone_cost = drone_cost; //Actualizamos el máximo
        }
        truck_cost = distance(o.start,o.end)*1000 / truck_speed; //Tiempo = distancia [km] / velocidad [m/s] = (distancia * 1000)[m]/velocidad[m/s]
        max_drone_cost = max_drone_cost*1000  / drone_speed;
        total_cost += max(truck_cost, max_drone_cost); //Sumamos el costo de la operación al costo total del recorrido
    }

    //Buscamos que todos los clientes de C_nodes hayan sido atendidos en la solución.
    bool found_all = true;
    for(customer_location c : C_nodes){
        bool found = false;
        for (customer_location d : clients_in_solution){
            if (d.l.x == c.l.x && d.l.y == c.l.y){
                found = true;
                break;
            }
        }
        if (!found){
            found_all = false;
            break;
        }
    }
    if (found_all) return total_cost; //Si todos los clientes fueron atendidos, la evaluación es el costo de los viajes
    else return numeric_limits<long double>::infinity(); //Si no, es infinito. (Solución infactible)
}


vector<customer_location> getNearbyClients(vector<customer_location> C_nodes, location l, long double range){
    vector<customer_location> nearby_clients;
    int size = C_nodes.size();
    for(int c = 0; c < size; c++){ //Encontramos clientes cercanos
        if (distance(C_nodes[c].l,l) <= range){
            nearby_clients.push_back(C_nodes[c]);
        }
    }
    return nearby_clients;
}

vector<operation> getOperations(vector<location> V_nodes, vector<customer_location> &C_nodes, int k, int capacity, float truck_speed, float drone_speed){
    vector<customer_location> local_C_nodes;
    long double best_eval = numeric_limits<long double>::infinity();
    long double current_eval;
    long double upper_range = 100;
    long double lower_range = 0;
    long double avg; //Comenzamos con un rango grande
    vector<operation> operations;
    vector<operation> optimal_range_operations;
    int i = 0;
    while(true){
        if (upper_range - lower_range < 0.05) break; //Terminamos cuando las cotas son justas.
        operations.clear();
        avg = abs(upper_range + lower_range)/2;
        local_C_nodes = C_nodes;
        int size = V_nodes.size();
        for(int i = 0; i < size; i++){
            vector<customer_location> clients = getNearbyClients(local_C_nodes, V_nodes[i], avg);
            operations.push_back(AssignDronesToClients(V_nodes[i],V_nodes[(i+1)%size],k,capacity,clients,local_C_nodes));
        }
        current_eval = eval(operations, C_nodes, truck_speed,drone_speed,k);
        if (current_eval <= best_eval){
            best_eval = current_eval;
            optimal_range_operations = operations;
            upper_range = avg; //Si el rango es bueno, ajustamos la cota superior (Busqueda Binaria del Rango)
        }
        else lower_range = avg; //Si el rango es demasiado pequeño, ajustamos la cota inferior
    }
    return optimal_range_operations;
}

vector<operation> removeUnnecessaryTrips(vector<operation> operations){
    for(int i = 0; i < operations.size(); i++){
        if (operations.at(i).clients_attended.empty()){
            operations.erase(operations.begin() + i);
            i--;
        }
    }
    return operations;
}

operation AssignDronesToClients(location V_start, location V_end, int k, int capacity, vector<customer_location> nearby_clients, vector<customer_location> &C_nodes){    
    //Creamos una lista de "drones" Ej: {5,5} para k=2 y capacity = 5
    vector<float> drones;
    for(int i = 0; i < k; i++){
        drones.push_back(capacity); 
    }
    vector<pair<int,customer_location>> clients_attended;
    int d = 0;
    bool full_capacity = false;
    for(customer_location c : nearby_clients){
        for (int i = 0; i < drones.size(); i++){
            int pos = (d+i) % drones.size(); //Usamos modulo para distribuir los clientes secuencialmente entre los drones
            if (c.mass <= drones.at(pos)){ //Para cada cliente, si su masa puede ser llevada por un dron, se escoge
                clients_attended.push_back(make_pair(pos,c));
                deleteClient(C_nodes,c.l.x,c.l.y);
                drones.at(pos) -= c.mass;
                d++;
                break;
            }
            if (i == drones.size() - 1) full_capacity = true; //Si todos los drones estan a full capacidad
        }
        if (full_capacity) break; //Si los drones estan a full capacidad, ya no se pueden asignar mas clientes
    }
    //Construimos la operación para retornarla
    operation op;
    op.start = V_start;
    op.end = V_end;
    op.clients_attended = clients_attended;
    return op;
}

//Supuesto: No hay 2 clientes en 1 mismo punto de coordenadas
void deleteClient(vector<customer_location> &C_nodes, long double x, long double y){
    vector<customer_location>::iterator iter;
    if (iter == C_nodes.end()){ //Si la lista esta vacia retornamos inmediatamente
        return;
    }
    for(int i = 0; i < C_nodes.size(); i++){
        if (C_nodes[i].l.x == x && C_nodes[i].l.y == y){ //Deberia encontrar 1 solo
            iter = C_nodes.begin() + i;
            C_nodes.erase(iter);
            return;
        }
    }
    cout << "ERROR: Delete client no encontró al cliente" << endl; //Nunca debería llegar a este punto
}  

//Supuesto: No hay 2 clientes en 1 mismo punto de coordenadas
void deleteVNode(vector<location> &V_nodes, long double x, long double y){
    vector<location>::iterator iter;
    if (iter == V_nodes.end()){ //Si la lista esta vacia retornamos inmediatamente
        return;
    }
    for(int i = 0; i < V_nodes.size(); i++){
        if (V_nodes[i].x == x && V_nodes[i].y == y){ //Deberia encontrar 1 solo
            iter = V_nodes.begin() + i;
            V_nodes.erase(iter);
            return;
        }
    }
}  


void drawOperations(vector<operation> operations, int k){
    ofstream arc("path.txt");
    int i = 0;
    arc << k << endl;
    for(operation o : operations){
        arc << "Operation: " << i++ << endl; //Numero de operacion
        arc << "Start: " << to_string(o.start.x) + " " + to_string(o.start.y) << endl; //Nodo inicial de operacion
        arc << "Amount: " << o.clients_attended.size() << endl; //Cuantos clientes se atienden en esta operacion
        for (pair<int,customer_location> p : o.clients_attended){
            arc << "Trip: " << p.first << " " << p.second.l.x << " " << p.second.l.y << endl;
        }
        arc << "--" << endl;
    }
    arc.close();
}