from os import system
from glob import glob

for f in glob('*.in'):
  system('./validator %s %s' % (f, f[:-3] + '.out'))
