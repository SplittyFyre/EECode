from os import times
import subprocess, sys

files = [i for i in range(1, 21)]
files += [i for i in range(36, 46)]

datas = []

times = 0
mems = []

print(sys.argv[1] + ': ')

x = 1
for i in files:
    p = subprocess.run('time {} < data/in{}.txt'.format(sys.argv[1], i),
        shell=True, executable='/bin/zsh', env={'TIMEFMT': '%*U %M'}, capture_output=True)
    with open('data/out{}.txt'.format(i), 'r') as fin:
        if p.stdout != fin.read().encode('ascii'):
            print('Error: WA on case {}/{}\n'.format(x, i))
            sys.exit()

    datas.append(p.stderr.decode('ascii'))

    info = p.stderr.decode('ascii').split(' ')
    times += round(float(info[0]) * 1000)
    mems.append(int(info[1]))

    x += 1

print('Total Time: {} milliseconds'.format(times))
print('Avg Mem: {} megabytes\n'.format(round(sum(mems) / len(mems))))
