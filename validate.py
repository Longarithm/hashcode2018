from os import system
from glob import glob
import os

system('g++ -g -std=c++14 validator.cpp -o validator -O2 -DLOCAL -Wall -Wextra')

for f in glob('*.in'):
    if os.name == 'nt':
        system('validator %s %s' % (f, f[:-3] + '.out'))
    else:
        system('./validator %s %s' % (f, f[:-3] + '.out'))

if os.name == 'nt':
    system('del validator.exe')
else:
    system('rm validator')
