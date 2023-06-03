// TODO: Add any extra #includes that you need

#include "pqueue.h"
#include "graph.h"
#include "pagerank.h"
#include "dijkstra.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// TODO: Add your data structure definitions
typedef struct vertex {
    string data;
    weight weight;
    double oldrank;
    double pagerank;
    struct vertex *next;
} vertex;

typedef struct GraphRepr {
    vertex **edge;
    size_t nv; //num of vertices
    int *pred;
} GraphRepr;

// TODO: Fill in these function stubs

graph graph_create(void) {
    graph g = malloc(sizeof(GraphRepr));
    assert(g != NULL);
    g->nv = 0;
    g->edge = NULL;
    return g;
}

void graph_destroy(graph g) {
    if (g == NULL) {
        return;
    }
    for (size_t i = 0; i < g->nv; i++) {
        vertex *curr = g->edge[i];
        while (curr != NULL) {
            vertex *temp = curr->next;
            free(curr->data);
            free(curr);
            curr = temp;
        }
    }
    free(g->edge);
    free(g);
}

void graph_show(graph g, FILE *output) {
    if (g == NULL) {
        return;
    }
    if (output == NULL) {
        output = stdout; //change the output target
    }
    
    for (size_t i = 0; i < g->nv; i++) {
        fprintf(output, "%s\n", g->edge[i]->data);
    }

    for (size_t i = 0; i < g->nv; i++) {
        vertex *temp = g->edge[i]->next;
        while(temp != NULL) {
            fprintf(output, "%s %s %.3f\n", g->edge[i]->data, temp->data, temp->weight);
            temp = temp->next; 
        }
    }

}

void graph_add_vertex(graph g, string v) {
    assert(g != NULL && v!= NULL && !graph_has_vertex(g,v));

    vertex **new_edge = realloc(g->edge, (g->nv + 1) * sizeof(vertex *));//extend length of first line
    assert(new_edge != NULL);

    g->edge = new_edge;
    g->edge[g->nv] = malloc(sizeof(vertex));
    assert(g->edge[g->nv] != NULL);
    g->edge[g->nv]->data = strdup(v); //make a copy
    g->edge[g->nv]->weight = 0.0;
    g->edge[g->nv]->oldrank = 0.0;
    g->edge[g->nv]->pagerank = 0.0;
    g->edge[g->nv]->next = NULL;
    g->nv++;
}

bool graph_has_vertex(graph g, string v) {
    if (g == NULL || v == NULL) {
        return false;
    }

    for (size_t i = 0; i < g->nv; i++) {
        if (strcmp(v, g->edge[i]->data) == 0) {
            return true;
        }
    }
    return false;
}

size_t graph_vertices_count(graph g) {
    if (g == NULL) {
        return 0;
    } else {
        return g->nv;
    }
}

bool graph_has_edge(graph g, string v1, string v2) {
    assert(g != NULL && v1 != NULL && v2 != NULL);

    if (strcmp(v1, v2) == 0) {
        return false;
    }

    for (size_t i = 0; i < g->nv; i++) {
        if (strcmp(g->edge[i]->data, v1) == 0) {
            vertex *temp = g->edge[i];
            while (temp != NULL) {
                if(strcmp(temp->data, v2) == 0) {
                    return true;
                }
                temp = temp->next;
            }
            return false;
        }
    }
    return false;
}

void graph_add_edge(graph g, string v1, string v2, weight w) {
    assert(g != NULL && v1 != NULL && v2 != NULL);
    if (!graph_has_vertex(g, v1) || !graph_has_vertex(g, v2)) {
        return;
    }
    if (graph_has_edge(g, v1, v2)) {
        return;
    }
    
    for (size_t i = 0; i < g->nv; i++) {
        if (strcmp(g->edge[i]->data, v1) == 0) {
            vertex *new = malloc(sizeof(vertex));
            assert(new != NULL);
            new->data = strdup(v2);
            new->weight = w;
            new->next = NULL;
            vertex *curr = g->edge[i];
            while(curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = new;
        }
    }
}

void graph_update_edge(graph g, string v1, string v2, weight w) {
    assert(g != NULL && v1 != NULL && v2 != NULL);

    if (!graph_has_edge(g, v1, v2)) {
        graph_add_edge(g, v1, v2, w);
        return;
    }

    for (size_t i = 0; i < g->nv; i++) {
        if (strcmp(g->edge[i]->data, v1) == 0) {
            vertex *curr = g->edge[i];
            while (strcmp(curr->data, v2) == 0) {
                curr = curr->next;
            }
            curr->weight = w;
            return;
        }
    }
}

weight graph_get_edge(graph g, string v1, string v2) {
    if (g == NULL || v1 == NULL || v2 == NULL) {
        return 0;
    }

    for (size_t i = 0; i < g->nv; i++) {
        if(strcmp(g->edge[i]->data, v1) == 0) {
            vertex *curr = g->edge[i];
            while (strcmp(curr->data, v2) == 0) {
                curr = curr->next;
            }
            return curr->weight;
        }
    }
    
    return 0;
}

size_t graph_edges_count(graph g, string v) {
    if (g == NULL || v == NULL || !(graph_has_vertex(g, v))) {
        return 0;
    }
    
    size_t count = 0;
    //outgoing
    for (size_t i = 0; i < g->nv; i++) {
        if(strcmp(g->edge[i]->data, v) == 0) {
            vertex *curr = g->edge[i];
            while (curr != NULL) {
                count++;
                curr = curr->next;
            }
        }
    }

    //incoming
    for (size_t i = 0; i < g->nv; i++) {
        vertex *temp = g->edge[i];
        if (strcmp(temp->data, v) == 0) {
            continue;
        } else {
            while (temp != NULL) {
                if(strcmp(temp->data, v) == 0){
                    count++;
                }
                temp = temp->next;
            }
        }
    }
    return count;
}

int num_outbound_edge(graph g, string v) {
    assert(g != NULL && v !=  NULL);
    int count = 0;

    for(size_t i = 0; i < g->nv; i++) {
        if (strcmp(g->edge[i]->data, v) == 0) {
            vertex *temp = g->edge[i];
            while (temp->next != NULL) {
                count++;
                temp = temp->next;
            }
            return count;
        }
    }
    return count;
}

void graph_pagerank(graph g, double damping, double delta) {
    size_t N = graph_vertices_count(g);
    
    for (size_t i = 0; i < N; i++) {
        g->edge[i]->oldrank = 0;
        g->edge[i]->pagerank = 1/N;
    }

    while(1) {
        for (size_t i = 0; i < N; i++) {
            g->edge[i]->oldrank = g->edge[i]->pagerank;
        }

        double sink_rank = 0.0;
        for (size_t i = 0; i < N; i++) {
            if (g->edge[i]->next == NULL) {
                sink_rank = sink_rank + (damping * (g->edge[i]->oldrank / N));
            }
        }

        for (size_t i = 0; i < N; i++) {
            g->edge[i]->pagerank = sink_rank + ((1 - damping) / N);
            for (size_t j = 0; j < N; j++) {
                if (graph_has_edge(g, g->edge[j]->data, g->edge[i]->data)) {
                    g->edge[i]->pagerank += ((damping * g->edge[j]->oldrank) / num_outbound_edge(g, g->edge[j]->data));
                }
            }
        }

        int check = 1;
        for (size_t i = 0; i < N; i++) {
            if(fabs(g->edge[i]->pagerank - g->edge[i]->oldrank) > delta) {
                check = 0;
                break;
            }
        }
        if(check == 1) {
            break;
        }
    }
}

int compare(const void *a, const void *b) {
    // const viod *a points to a vertex pointer which needs to be sorted in the edge list.
    // so use v1 to get address which a stores.
    const vertex *v1 = *(const vertex **)a;
    const vertex *v2 = *(const vertex **)b;

    if(v1->pagerank > v2->pagerank) {
        return -1;
    } else if(v1->pagerank < v2->pagerank) {
        return 1;
    } else {
        return strcmp(v1->data, v2->data);
    }
}

void graph_show_pagerank(graph g, FILE *file) {
    assert(g != NULL);

    qsort(g->edge, g->nv, sizeof(vertex *), compare);

    for(size_t i = 0; i < g->nv; i++) {
        fprintf(file, "%s (%.3f)\n", g->edge[i]->data, g->edge[i]->pagerank);
    }
}

size_t graph_vertex_index(graph g, string v) {
    assert(g != NULL && v != NULL);
    size_t index = -1;
    for(size_t i = 0; i< g->nv; i++) {
        if(strcmp(g->edge[i]->data, v) == 0) {
            index = i;
            return i;
        }
    }
    return index;
}

size_t find_min_index(graph g, int vSet[]) {
    size_t i;
    size_t min_index = -1;
    double min_pagerank = 999.0;
    for(i = 0; i < g->nv; i++) {
        if(vSet[i] == 0 && g->edge[i]->pagerank < min_pagerank) {
            min_index = i;
            min_pagerank = g->edge[i]->pagerank;
        }
    }
    return min_index;
}

void graph_worst_path(graph g, string source, double damping, double delta) {
    assert(g != NULL && source != NULL);
    if(!graph_has_vertex(g, source)) {
        return;
    }
    size_t source_index = graph_vertex_index(g, source);

    graph_pagerank(g, damping, delta);

    double dist[g->nv];
    g->pred = malloc(g->nv * sizeof(int));
    assert(g->pred != NULL);
    int visited[g->nv];
    size_t s;

    for (s = 0; s < g->nv; s++){
        dist[s] = 99.0;  
        g->pred[s] = -1;
        visited[s] = 0;
    }
    dist[source_index] = 0.0;

    for (size_t i = 0; i < g->nv; i++) {
        int min_index = -1;
        for(size_t j = 0; j < g->nv; j++) {
            if(!visited[j] && (min_index == -1 || dist[j] < dist[min_index])) {
                min_index = j;
            }
        }

        if(dist[min_index] == 99.0) {
            break;
        }

        visited[min_index] = 1;
        
        for(size_t j = 0; j < g->nv; j++) {
            if(graph_has_edge(g, g->edge[min_index]->data, g->edge[j]->data) && !visited[j]) {
                if (dist[min_index] + g->edge[j]->pagerank < dist[j]) {
                    dist[j] = dist[min_index] + g->edge[j]->pagerank;
                    g->pred[j] = min_index;
                }
            }
        } 
    }
}

void graph_show_path(graph g, string destination) {
    assert(g != NULL && destination != NULL);
    if(!graph_has_vertex(g, destination)) {
        return;
    }

    size_t N = g->nv;
    int num = (int)N;

    int dest_index = -1;
    int i;
    for(i = 0; i < num; i++) {
        if(strcmp(g->edge[i]->data, destination) == 0) {
            dest_index = i;
            break;
        }
    }

    if (g->pred[dest_index] == -1) {
        printf("No path found.\n");
        return;
    }

    int path[g->nv];
    int length = 0;
    path[length] = dest_index;


    int curr_index = dest_index;
    while(curr_index != -1) {
        curr_index = g->pred[curr_index];
        length++;
        path[length] = curr_index;
    }

    for(i = length - 1; i >= 0; i--) {
        printf("%s\n", g->edge[path[i]]->data);
        if(i != 0) {
            printf("-> ");
        }
    }
}
