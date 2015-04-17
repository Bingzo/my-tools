#! /usr/bin/env python
#coding=utf-8

def floatrange(start,stop,steps):
    ''' Computes a range of floating value.

        Input:
            start (float)  : Start value.
            end   (float)  : End value
            steps (integer): Number of values

        Output:
            A list of floats

        Example:
            >>> print floatrange(0.25, 1.3, 5)
            [0.25, 0.51249999999999996, 0.77500000000000002, 1.0375000000000001, 1.3]
    '''
    return [start+float(i)*(stop-start)/(float(steps)-1) for i in range(steps)]


inf_dos = open('DOSCAR')
inf_pos = open('CONTCAR')

#outf = open('LDOS-partial.dat','w')
outf2 = open('LDOS-total.dat','w')

#start = int(raw_input('please input the initial atom: '))
#end = int(raw_input('please input the last selected atom: '))
#startenergy = float(raw_input('please input the lower limit energy: '))
#endenergy = float(raw_input('please input the upper limit energy: '))
start = 1
end = 83
startenergy = -3.0
endenergy = 3.0
natom = end - start + 1

#obtain the position for selected atoms: Y, Z
inf_pos.readline()
constant = float(inf_pos.readline().strip())

inf_pos.readline()
vect_y = float(inf_pos.readline().split()[1]) * constant
vect_z = float(inf_pos.readline().split()[2]) * constant

for i in range(start + 3):
    inf_pos.readline()

pos = []
for i in range(end-start+1):
    line = inf_pos.readline().split()
    y = float(line[1]) * vect_y
    z = float(line[2]) * vect_z
    pos.append([y,z])

#obtain the DOS for selected atoms: DOS
for i in range(5):
    inf_dos.readline()

line = inf_dos.readline().split()
E_start = float(line[1])
E_end = float(line[0])
E_points = int(line[2])
E_fermi = float(line[3])
E_low = startenergy + E_fermi
E_up = endenergy + E_fermi

dE = (E_end - E_start)/(E_points - 1)

#print E_start, E_end, E_points, E_fermi, dE

point_fermi = int((E_up - E_low)/dE)
point_low = int((E_low - E_start)/dE)
point_up = E_points - point_fermi - point_low

E = []
pzDOS = []

for i in range(start * (E_points+1)):
    inf_dos.readline()

#print inf_dos.readline()

for i in range(natom):
    for i in range(point_low):
        inf_dos.readline()
#   print inf_dos.readline()
    for i in range(point_fermi):
        line = inf_dos.readline().split()
        E.append(float(line[0]) - E_fermi)
        pzDOS.append(float(line[4]))
    for i in range(point_up + 1):
        inf_dos.readline()

#print inf_dos.readline()
#print E[0]
#print pzDOS[0]

#output the position and LDOS for selected atoms: Z, Y ,E, pzDOS
#for i in range(natom):
#    for j in range(point_fermi):
#       N = j + i*point_fermi
#       outf.write("%.3f   %.3f   %.3f   %.3f\n"%(pos[i][1], pos[i][0], E[N], pzDOS[N]))

step = 60
DATA = []
pzTDOS = [[0 for col in range(point_fermi)] for row in range(step)]
ZCoor = floatrange(0,vect_z,step)
#ECoor = floatrange(E[0],E[point_fermi-1],point_fermi)

#for i in xrange(natom):
#    for j in xrange(point_fermi):
#        for k in xrange(step):
#            for l in xrange(point_fermi):
#                N = j + i*point_fermi
#                if abs(pos[i][1] - ZCoor[k]) < 0.25 and abs(E[N] - ECoor[l]) < 0.005 :
#                    pzTDOS[k][l] = pzTDOS[k][l] + pzDOS[N]

for i in xrange(natom):
        for k in xrange(step):
            if abs(pos[i][1] - ZCoor[k]) < 0.25:
                for l in xrange(point_fermi):
                    pzTDOS[k][l] = pzTDOS[k][l] + pzDOS[i*point_fermi+l]


#print DATA[0][0]
#step = 299
#ZCoor = floatrange(0,vect_z,step)
#ECoor = floatrange(E[0],E[point_fermi-1],point_fermi)
#print ZCoor
#print ECoor[0], ECoor[point_fermi-1]

#pzTDOS = [[0 for col in range(point_fermi)] for row in range(step)]
#for i in range(step):
#    for j in range(point_fermi):
#        for k in range(natom*point_fermi):
#            if abs(DATA[k][0] - ZCoor[i]) < 0.05 and abs(DATA[k][2] - ECoor[j]) < 0.005 :
#               pzTDOS[i][j] = pzTDOS[i][j] + DATA[k][3]

for i in range(step):
    for j in range(point_fermi):
       if pzTDOS[i][j] > 0.00001:
          outf2.write("%.3f   %.3f   %.3f \n"%(ZCoor[i], E[j], pzTDOS[i][j]))

inf_dos.close()
inf_pos.close()
#outf.close()
outf2.close()

