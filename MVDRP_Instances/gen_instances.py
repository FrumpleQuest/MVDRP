import numpy as np
import os
import pathlib

# random seed for replicate
np.random.seed(7)

# 25 km x 25 km square region
MAX_X = 25
MAX_Y = 25

# launch/retrieval locations
V_VALUES = [25,50]

# customer locations
C_VALUES= [25,50]

def create_instances(max_x,max_y,v_val,c_val):
    # list with all instances
    instances = []

    for v in v_val:
        for c in c_val:
            for i in range(25):
                # random uniform locations 
                locs_v = np.array([np.random.uniform(0,max_x,v),np.random.uniform(0,max_y,v)]).T
                locs_c = np.array([np.random.uniform(0,max_x,c),np.random.uniform(0,max_y,c)]).T


                packages_mass = np.random.uniform(0,2.3,c)
                depot = locs_v[np.random.randint(v)]
                instances.append([locs_v,locs_c,packages_mass,depot])

    # folder path
    current_path = pathlib.Path(__file__).parent.resolve()
    folder_path = os.path.join(current_path,'MVDRP_Instances')

    if not os.path.isdir(folder_path):
        os.mkdir(folder_path)

    # writing instances
    for idx,ins in enumerate(instances):

        locs_v,locs_c,packages_mass,depot = ins

        f_ins = open(os.path.join(folder_path,'instance %d.txt' % (idx+1)),'w')
        f_ins.write('|V|='+str(len(locs_v))+' |C|='+str(len(locs_c))+'\n')
        f_ins.write('Truck speed \n5.0 \n')
        f_ins.write('Drone speed \n10.0 \n')
        f_ins.write('Location of Depot \n')
        f_ins.write(str(depot[0])+' '+str(depot[1])+'\n')
        f_ins.write('launch/retrieval locations (V), in format (x,y) \n')
        for loc_v_i in locs_v:
            f_ins.write(str(loc_v_i[0])+' '+str(loc_v_i[1])+'\n')
        f_ins.write('customer locations (C) and packages mass, in format (x,y,mass) \n')
        for idx_i,loc_c_i in enumerate(locs_c):
            f_ins.write(str(loc_c_i[0])+' '+str(loc_c_i[1])+' '+str(packages_mass[idx_i])+'\n')
        f_ins.close()

if __name__ == '__main__':
    create_instances(MAX_X,MAX_Y,V_VALUES,C_VALUES)