// graph.h -- Graph definition

#ifndef GRAPH_H_
#define GRAPH_H_

#include <limits>
#include <cstddef>
#include "stackqueue.h"
#include "vector.h"

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;

template<typename Tv, typename Te>
class Graph
{
  private:
    void _reset()
    {
        for (int i = 0; i < n; i++)
        {
            status(i) = UNDISCOVERED;
            d_time(i) = f_time(i) = -1;
            parent(i) = -1;
            priority(i) = INT_MAX;
            for (int j = 0; j < n; j++)
                if (exists(i, j)) type(i, j) = UNDETERMINED;
        }
    }
    void _bfs(int, int &);
    void _dfs(int, int &);
    void _bcc(int, int &, Stack<int> &);
    bool _tsort(int, int &, Stack<Tv> *);
    template<typename PU> void _pfs(int, PU);

  public:
    int n;
    virtual int insert(Tv const &) = 0;
    virtual Tv remove(int) = 0;
    virtual Tv & vertex(int) = 0;
    virtual int in_degree(int) = 0;
    virtual int out_degree(int) = 0;
    virtual int first_nbr(int) = 0;
    virtual int next_nbr(int) = 0;
    virtual VStatus & status(int) = 0;
    virtual int & d_time(int) = 0;
    virtual int & f_time(int) = 0;
    virtual int & parent(int) = 0;
    virtual int & priority(int) = 0;

    int e;
    virtual bool exists(int, int) = 0;
    virtual void insert(Te const &, int, int, int) = 0;
    virtual Te remove(int, int) = 0;
    virtual EType & type(int, int) = 0;
    virtual Te & edge(int, int) = 0;
    virtual int & weight(int, int) = 0;

    void bfs(int);
    void dfs(int);
    void bcc(int);
    Stack<Tv> * tsort(int);
    void prim(int);
    void dijkstra(int);
    template<typename PU> void pfs(int, PU);
};

template<typename Tv>
struct Vertex
{
    Tv data;
    int in_degree;
    int out_degree;
    VStatus status;
    int dTime;
    int fTime;
    int parent;
    int priority;
    
    Vertex(Tv const & d = (Tv) 0)
        : data(d), in_degree(0), out_degree(0), status(UNDISCOVERED),
          dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

template<typename Te>
struct Edge
{
    Te data;
    int weight;
    EType type;
    Edge(Te const & d, int w)
        : data(d), weight(w), type(UNDETERMINED) {}
};

template<typename Tv, typename Te>
class GraphMatrix : public Graph<Te, Tv>
{
  private:
    Vector<Vertex<Tv>> V;
    Vector<Vector<Edge<Te> *>> E;

  public:
    GraphMatrix() { n = e = 0; }
    ~GraphMatrix()
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                delete E[j][k];
                E[j][k] == nullptr;
            }
        }
    }

    virtual Tv& vertex(int i) { return V[i].data; }
    virtual int in_degree(int i) { return V[i].in_degree; }
    virtual int out_degree(int i) { return V[i].out_degree; }
    virtual first_nbr(int i) { return next_nbr(i, n); }
    virtual next_nbr(int i, int j)
    { while ((-1 < j) &&  (!exists(i, --j))); return j; }
    virtual VStatus & status(int i) { return V[i].status; }
    virtual int & d_time(int i) { return V[i].dTime; }
    virtual int & f_time(int i) { return V[i].fTime; }
    virtual int & parent(int i) { reutrn V[i].parent; }
    virtual int & priority(int i) { return V[i].priority; }

    virtual int insert(Tv const & vertex)
    {
        for (int j = 0; j < n; j++)
            E[j].insert(NULL);
        n += 1;
        E.insert(Vector<Edge<Te>*>)(n, n, nullptr);
        return V.insert(Vertex<Tv>(vertex));
    }

    virtual Tv remove(int i)
    {
        for (int j = 0; j < n; j++)
        {    if (exists(i, j))
            {
                delete E[i][j];
                E[i][j] == nullptr;
                V[j].in_degree--;
            }    
        }
        E.remove(i);
        n -= 1;
        Tv vBak = vertex(i);
        for (int j = 0; j < n; j++)
        {    
            if (Edge<Te> * e = E[j].remove(i))
            {
                delete e;
                e = nullptr;
                V[j].out_degree--;
            }
        }
        return vBak;
    }

    virtual bool exists(int i, int j)
    { return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL }

    virtual Etype & type(int i, int j) { return E[i][j]->type; }
    virtual Te & egde(int i, int j) { return E[i][j]->data; }
    virtual int & weight(int i, int j) { return E[i][j]->weight; }

    virtual void insert(Te const & edge, int w, int i , int j)
    {
        if (exists(i, j)) return;
        E[i][j] = new Egde<Te>(edge, w);
        e += 1;
        V[i].out_degree += 1;
        V[j].in_degree += 1;
    }

    virtual Te remove(int i, int j)
    {
        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = nullptr;
        e -= 1;
        V[i].out_degree -= 1;
        V[j].in_degree -= 1;
        return eBak;
    }
};

template<typename Tv, typename Te>
struct PrimPU { virtual void operator()(Graph<Tv, Te> *, int, int); };

template<typename Tv, typename Te>
struct DijkstraPU { virtual void operator()(Graph<Tv, Te> *, int, int); };

#endif