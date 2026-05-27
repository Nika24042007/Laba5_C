#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graf{
    int V;
    int R_M;
    int R;
    int **matrix;
} Graf;

Graf *create_graf(int v, int m_e){
    Graf *g = malloc(sizeof(Graf));
    if (g == NULL) return NULL;
    g->V = v;
    g->R_M = m_e;
    g->R = 0;
    g->matrix = malloc(v*sizeof(int*));
    if (g->matrix == NULL){
        free(g);
        return NULL;
    }
    for (int i=0; i<v; i++){
        g->matrix[i] = calloc(m_e, sizeof(int));
    }
    return g;
}

void add_vertex(Graf *g){
    int new = g->V+1;
    int r_m_new= (new *(new-1))/2;
    for (int i = 0; i < g->V; i++){
        int *new_row = realloc(g->matrix[i], r_m_new*sizeof(int));
        if(new_row == NULL){
            printf("Memory error\n");
            return;
        }
        for(int j= g->R_M; j < r_m_new; j++){
            new_row[j] = 0;
        }
        g->matrix[i]= new_row;
    }
    int **newmatrix = realloc(g->matrix, new*sizeof(int*));
    if (newmatrix == NULL){
        printf("Memory error\n");
        return;
    }
    g->matrix = newmatrix;
    g->matrix[g->V] = calloc(r_m_new, sizeof(int));
    g->V = new;
    g->R_M = r_m_new;
    printf("New vertex (%d) added\n", new);
}

void add_edge(Graf *g, int u, int v){
    if (g->R < g->R_M){
        if (u > -1 && u < g->V && v>-1 && v < g->V){
            g->matrix[u][g->R] = 1;
            g->matrix[v][g->R] = -1;
            g->R++;
            printf("New edge edded\n");
        }else{
            scanf("No, such vertex\n");
        }
    }else{
        printf("there is no place for an edge. add a vertex\n");
    }
}

void delete_vertex(Graf *g, int idx){
    if (g == NULL || idx < 0 || idx >= g->V){
        printf("No such index of vertex\n");
        return;
    }
    int left = 0;
    for (int i = 0; i < g->R; i++){
        if (g->matrix[idx][i] == 0){
            for (int j = 0; j < g->V; j++){
                g->matrix[j][left] = g->matrix[j][i];
            }
            left++;
        }
    }
    g->R = left;
    free(g->matrix[idx]);
    for (int i=idx; i <g->V-1; i++){
        g->matrix[i] = g->matrix[i+1];
    }
    g->V--;
    if (g->V > 0){
        int **new_m = realloc(g->matrix,g->V*sizeof(int*));
        if (new_m == NULL){
            printf("Memory error\n");
            return;
        }
        g->matrix = new_m;
    }
    int r_m_new = (g->V *(g->V-1))/2;
    for (int i = 0; i < g->V; i++){
        int *new_row = realloc(g->matrix[i], r_m_new*sizeof(int));
        if (new_row == NULL){
            printf("Memory error\n");
            return;
        }
        g->matrix[i] = new_row;
    }
    g->R_M = r_m_new;
    printf("Vertex deleted\n");
}

void delete_edge(Graf *g, int idx){
    if (idx < 0 || idx > g->R){
        printf("No such index of vertex\n");
        return;
    }
    for (int i = 0; i < g->V; i++){
        g->matrix[i][idx] = 0;
    }
    for (int i = idx; i < g->R-1; i++){
        for(int j = 0; j < g->V; j++){
            g->matrix[j][i] = g->matrix[j][i+1];
        }
    }
    g->R--;
    printf("Edge deleted\n");
}

Graf *copygraf(Graf *g){
    Graf *copy = create_graf(g->V, g->R_M);
    copy->R = g->R;
    for (int i = 0; i < g->V; i++){
        for (int j = 0; j < g->R; j++){
            copy->matrix[i][j] = g->matrix[i][j];
        }
    }
    return copy;
}

void print_matrix(Graf *g){
    printf("Matrix (%d x %d):\n", g->V, g->R);
    printf("       ");
    for (int j = 0; j < g->R; j++){
        printf("e%-2d ", j);
    }
    printf("\n");
    for (int i = 0; i<g->V; i++){
        printf("v%-2d |", i);
        for (int j=0; j < g->R; j ++){
            printf("%4d", g->matrix[i][j]);
        }
        printf("\n");
    }
}

void sort_graf(Graf *g){
    Graf *cg = copygraf(g);
    int *sort_list = malloc(cg->R * sizeof(int));
    int count = 0;
    int c = -1;
    int num = cg->V;
    while (count != num && c != count){
        c = count;
        for(int i = 0; i < cg->V; i++){
            int f = 0;
            if(i == 14){
                int z = 0;
            }
            for (int j = 0; j < cg->R; j++){
                if(cg->matrix[i][j] == -1){
                    f = 1;
                    break;
                }
            }
            if (f == 0){
                int k = 0;
                for (int j=0; j < count; j++){
                    if(sort_list[j] == i){
                        k = 1;
                        break;
                    }
                }
                if(k == 0){
                    sort_list[count] = i;
                    count++;
                    int *one = malloc(cg->V*sizeof(int));
                    int n = 0;
                    for(int j = 0; j < cg->R; j++){
                        if(cg->matrix[i][j]==1){
                            one[n] = j;
                            n++;
                        }
                    }
                    for(int j =0; j < n; j++){
                        delete_edge(cg, one[j]-j);
                    }
                    free(one);
                }
            }
        }
    }
    if (count == num){
        for (int i = 0; i < count; i++){
            printf("%d ", sort_list[i]);
        }
        printf("\n");
    }else{
        printf("There is a cycle in the graph\n");
    }
}
void free_graf(Graf *g){
    for (int i = 0; i < g->V; i++){
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
}


void main(){
    Graf *graf = create_graf(2, 1);
    if(graf == NULL) return;
    int f=1;
    int c;
    printf("Enter command (0.Exit  1.Add vertex  2.Add edge  3.Delete vertex  4.Delete edge  5.Sort graf  6.Print): ");
    scanf("%d", &c);
    printf("\n");
    while(f==1){
        if(c==0){
            f = 0;
        }else if(c==1){
            add_vertex(graf);
        }else if(c==2){
            int u, v;
            printf("Enter number of first vertex from: ");
            scanf("%d", &u);
            printf("\n");
            printf("Enter number of end fertex to: ");
            scanf("%d", &v);
            printf("\n");
            add_edge(graf, u, v);
        }else if(c==3){
            int idx;
            printf("Enter index of vertex: ");
            scanf("%d", &idx);
            printf("\n");
            delete_vertex(graf, idx);
        }else if(c==4){
            int idx;
            printf("Enter index of edge: ");
            scanf("%d", &idx);
            printf("\n");
            delete_edge(graf, idx);
        }else if(c==5){
            sort_graf(graf);
        }else if(c==6){
            print_matrix(graf);
        }else{
            printf("No such command\n");
        }
        if(f != 0){
            printf("Enter command (0.Exit  1.Add vertex  2.Add edge  3.Delete vertex  4.Delete edge  5.Sort graf  6.Print): ");
            scanf("%d", &c);
            printf("\n");
        }
    }
    free_graf(graf);
}