from sets import Set

vertices_num = int(raw_input("how many vertices?\n"))
result_filename = "twitter-{}.txt".format(vertices_num)


f = open('twitter-weighted.txt', 'r')
f_res = open(result_filename, 'w')

vertices = Set()
edges_num = 0
maxV = 0
for line in f:
    line_spl=line.split()
    vertices.add(int(line_spl[0]))
    vertices.add(int(line_spl[1]))
    if len(vertices) > vertices_num:
        break
    edges_num += 1
    maxV = max(maxV, max(int(line_spl[0]), int(line_spl[1])))
    f_res.write(line)

f.close()
f_res.close()

print("add to first line: {} {} {}".format(maxV, len(vertices)-2, edges_num))
