from os import system
from glob import glob

system('g++ -g -std=c++14 validator.cpp -o validator -O2 -DLOCAL -Wall -Wextra')

for f in glob('*.in'):
  system('./validator %s %s' % (f, f[:-3] + '.out'))

system('rm validator')
