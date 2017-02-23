fin = open("cookies.in", 'r')
fout = open("cookies.out", 'w')

n = int(fin.readline().strip())
two, one = tupple(map(int, fin.readline().strip()))
fout.write(n - 2 * two - one)
fin.close()
fout.close()