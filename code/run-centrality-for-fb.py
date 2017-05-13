import facebook
import subprocess

# put your token here
TOKEN = "EAACEdEose0cBAFVs9RpZCcceXLQOymeTI6ZCyh6VF4rnJEgeKqZBH64VovmKvSNn4b2jQVcCRZBu49aHYdXVndH2gfVnny8htWgQS6ezZAZBroePCliJzdUZCC7t9fSODwc2Q2gsJ5XHGEA6er4vcPnZBZCgZCjg6iMH2u9X3NNgZBaZBTHSZAj4xblKQEroAHgXosHcZD"
graph = facebook.GraphAPI(access_token=TOKEN, version='2.9')

GRAPH_FILENAME = 'fb-graph.txt'
INDEX_TO_NAME = 'fb-index-to-name.txt'
RESULTS_FILENAME = 'fb-centrality-results.txt'

file_handle = open(GRAPH_FILENAME, 'w')
file_handle2 = open(INDEX_TO_NAME, 'w')

num_vertices = 0
id_to_index = dict()
id_to_name = dict()
visited = set()
adj_dict = dict()
index_to_name = dict()

UNWEIGHTED = 11
DISCONNECTED = 12


def get_index(id1):
    global num_vertices
    if id1 in id_to_index:
        index = id_to_index[id1]
    else:
        index = num_vertices
        id_to_index[id1] = index
        num_vertices += 1
    return index


def make_graph(my_id, depth):
    if depth == 2:
        return
    my_friends = graph.get_all_connections(id=my_id, connection_name='friends')
    my_index = get_index(my_id)

    visited.add(my_index)
    print("visited your friend {}".format(id_to_name[my_id].encode('utf8')))

    adj_dict[my_index] = []
    for f in my_friends:
        friend_id = f['id']
        friend_name = f['name']
        id_to_name[friend_id] = friend_name
        friend_index = get_index(friend_id)
        if friend_index not in visited:
            adj_dict[my_index].append(friend_index)
            index_to_name[friend_index] = friend_name
            make_graph(friend_id, depth+1)


def add_weights(adj_matrix):
    for src in xrange(num_vertices):
        for dst in xrange(src+1, num_vertices):
            if adj_matrix[src][dst] == UNWEIGHTED:
                counter = 0
                for i in xrange(num_vertices):
                    if i!=src and i!=dst and adj_matrix[src][i] != DISCONNECTED and adj_matrix[dst][i] != DISCONNECTED:
                        counter+=1
                if counter == 0:
                    counter = 1
                adj_matrix[src][dst] = adj_matrix[dst][src] = int(round(1.0/counter*10))


def write_edges(adj_matrix):
    for src in xrange(num_vertices):
        for dst in xrange(src+1, num_vertices):
            if adj_matrix[src][dst] != DISCONNECTED:
                file_handle.write("{} {} {}\n".format(src, dst, adj_matrix[src][dst]))
    file_handle.close()


def main():
    me = graph.get_object(id='me')
    my_id = me["id"]
    my_name = me["name"]
    id_to_name[my_id] = my_name
    index_to_name[0] = my_name
    make_graph(my_id, 0)

    adj_matrix = []
    for row in range(num_vertices):
        adj_matrix += [[DISCONNECTED]*num_vertices]

    for src in adj_dict:
        for dst in adj_dict[src]:
            adj_matrix[src][dst] = adj_matrix[dst][src] = UNWEIGHTED

    add_weights(adj_matrix)

    write_edges(adj_matrix)

    for index in index_to_name:
        file_handle2.write("{};{}\n".format(index, index_to_name[index].encode('utf8')))
    file_handle2.close()

    args = ['./centrality-omp-fb', GRAPH_FILENAME, str(num_vertices), '1', INDEX_TO_NAME, RESULTS_FILENAME]
    p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    output_file = p.stdout
    for i in xrange(2):
        print(output_file.readline().rstrip())


if __name__ == '__main__':
    main()

    # num_vertices = 722
    # args = ['./centrality-executable', GRAPH_FILENAME, str(num_vertices), '1', INDEX_TO_NAME, RESULTS_FILENAME]
    # p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    # output_file = p.stdout
    # for i in xrange(2):
    #     print(output_file.readline().rstrip())
