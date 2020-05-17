# -*- coding: utf-8 -*-
"""
A program to determine alpha and theta angles form by a given inclined plan. Such plan is define by three
coordinates {x,y,z} 
"""

import sympy as sp

# Define referential system
N = sp.vector.CoordSys3D('N')

# Create three coordinates that define the inclined plan
x1, y1, z1 = sp.symbols('x1 y1 z1')
x2, y2, z2 = sp.symbols('x2 y2 z2')
x3, y3, z3 = sp.symbols('x3 y3 z3')

p1 = x1*N.i + y1*N.j + z1*N.k
p2 = x2*N.i + y2*N.j + z2*N.k
p3 = x3*N.i + y3*N.j + z3*N.k

# Calculate two orthogonal vectors that define the inclined plan
PQ = p1 - p2
PR = p3 - p2

PR.cross(PQ)
# Calculate the vector normal to the inclined plan
n = sp.vector.cross(PR,PQ)

# Define two vectors which are normal to XZ and YZ plan
n1 = N.j
n2 = N.i

# Vecteurs donnant la direction de l'intersection entre 
vA = sp.vector.cross(n,n1)
vB = sp.vector.cross(n,n2)

# Calcul des normes des vecteurs n, vA et vB
norm_vA = sp.sqrt(vA.args[0].args[0]*vA.args[0].args[0] + vA.args[1].args[0]*vA.args[1].args[0])
norm_vB = sp.sqrt(vB.args[0].args[0]*vB.args[0].args[0] + vB.args[1].args[0]*vB.args[1].args[0])

# a . b = ||a|| . ||b|| . cos(angle) -> calcul de cos(angle)
 
 
cos_theta = sp.vector.dot(N.i,vA)/(norm_vA)
cos_alpha = sp.vector.dot(N.j,vB)/(norm_vB)

# Calculate angles of the inclined plan base on a rotation around X axis (alpha) or Y axis (theta)
# Calcul des angles d'inclinaison du plan incliné selon une rotation autours des axes des X (alpha) et des Y (theta)
theta = sp.acos(cos_theta)
alpha = sp.acos(cos_alpha)

sp.init_printing()

theta
alpha