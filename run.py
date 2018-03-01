from os import system
from glob import glob

for f in glob('*.in'):
  system('main.exe < %s > %s' % (f, f[:-3] + '.out'))
