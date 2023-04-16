import matplotlib.pyplot as plt

arc = open("path.txt")

operations = list()
in_op = False
string = arc.readline()
k = int(string.strip())
string = arc.readline()
while(string):
    string = string.strip().split(" ")
    if (string[0] == "Operation:"):
        in_op = True
    elif (string[0] == "Start:"):
        start = tuple((float(string[1]),float(string[2])))
    elif (string[0] == "Amount:"):
        i = int(string[1])
        trips = list()
        while(i>0):
            trip = arc.readline()
            trip = trip.strip().split(" ")
            trips.append(tuple((int(trip[1]),float(trip[2]),float(trip[3]))))
            i -= 1
    elif (string[0] == "--"):
        operations.append(tuple((tuple(start),list(trips))))
    string = arc.readline()

vxs = list()
vys = list()
cxs = list()
cys = list()
drone_routes_x = [[] for i in range(0,k)]
drone_routes_y = [[] for i in range(0,k)]
for op in operations:
    vxs.append(op[0][0])
    vys.append(op[0][1])
    c = 0
    for lista in drone_routes_x:
        lista.append(op[0][0])
        i = 0
        while(i < len(op[1])):
            if(op[1][i][0] == c):
                lista.append(op[1][i][1])
                cxs.append(op[1][i][1])
            i += 1
        c += 1
    c = 0
    for lista in drone_routes_y:
        lista.append(op[0][1])
        i = 0
        while(i < len(op[1])):
            if(op[1][i][0] == c):
                lista.append(op[1][i][2])
                cys.append(op[1][i][2])
            i += 1
        c += 1
arc.close()
plt.plot(vxs,vys,marker='D', linewidth = 2)
plt.plot(vxs[0],vys[0],marker='s', markersize=8)
plt.plot(cxs,cys,linestyle = "None",marker='s')
drone_legends = []
i = 0
while(i<len(drone_routes_x)):
    plt.plot(drone_routes_x[i],drone_routes_y[i],markersize=3,linewidth=0.5)
    drone_legends.append('Drone ' + str(i))
    i += 1
plt.legend(['V Nodes','Start Node','C Nodes'] + drone_legends) 
plt.show()
