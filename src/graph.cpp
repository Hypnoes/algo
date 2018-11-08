// graph.cpp -- Graph class
#include "graph.h"

template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
    _reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
            _bfs(v, clock);
    } while (s!=(v=(++v % n)));
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::_bfs(int v, int & clock)
{
    Queue<int> Q;
    status(v) = DISCOVERED;
    Q.enqueue(v);
    while (!Q.empty())
    {
        int v = Q.dequeue();
        d_time(v) = ++clock;
        for (int u = first_nbr(v); -1 < u; u = next_nbr(v, u))
        {
            if (UNDISCOVERED == status(u))
            {
                status(u) = DISCOVERED;
                Q.enqueue(u);
                type(v, u) = TREE;
                parent(u) = v;
            }
            else
            {
                type(v, u) = CROSS;
            }
        }
        status(v) = VISITED;
    }
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
    _reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
            _dfs(v, clock);        
    } while (s!=(v=(++v % n)));
    
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::_dfs(int v, int & clock)
{
    d_time(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = first_nbr(v); -1 < u; u = next_nbr(v, u))
    {
        switch (status(v))
        {
            case UNDISCOVERED:
                type(v, u) = TREE;
                parent(U) = v;
                _dfs(u, clock);
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD;
                break;
            default:
                type(v, u) = (d_time(v) < d_time(u)) ? FORWARD : CROSS;
                break;
        }
    }
    status(v) = VISITED;
    f_time(v) = ++clock;
}

template<typename Tv, typename Te>
bool Graph<Tv, Te>::_tsort(int v, int & clock, Stack<Tv> * S)
{
    d_time(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = first_nbr(v); -1 < u; u = next_nbr(v, u))
    {
        switch (status(u))
        {
            case UNDISCOVERED:
                parent(u) = v;
                type(v, u) = TREE;
                if (!_tsort(u, clock, S))
                    return false;
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD;
                return false;
            default:
                type(v, u) = (d_time(v) < d_time(u)) ? FORWARD  : CROSS;
                break;
        }
    }
    status(v) = VISITED;
    S->push(vertex(v));
    return true;
}

template<typename Tv, typename Te>
Stack<Tv> * Graph<Tv, Te>::tsort(int s)
{
    _reset();
    int clock = 0;
    int v = s;
    Stack<Tv> * S = new Stack<Tv>;
    do
    {
        if (UNDISCOVERED == status(v))
        {
            if (!_tsort(v, clock, S))
            {
                while (!S->empty())
                { 
                    S->pop();
                    break;
                }
            }
        }
    } while (s != (v=(++v % n));
    status(v) = VISITED;
    S->push(vertex(v));
    return true;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
    _reset();
    int clock = 0;
    int v = s;
    Stack<int> S;
    do
    {
        if (UNDISCOVERED == stauts(v))
        {   
            _bcc(v, clock, S);
            S.pop();
        }
    } while (s != (v = (++v % n)));
}

#define hca(x) (f_time(x))
template<typename Tv, typename Te>
void Graph<Tv, Te>::_bcc(int v, int & clock, Stack<int> & S)
{
    hca(v) = d_time(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v);
    for (int u = first_nbr(v); -1 < u; u = next_nbr(v, u))
    {
        switch (status(u))
        {
            case UNDISCOVERED:
                parent(u) = v;
                type(v, u) = TREE;
                _bcc(u, clock, S);
                if (hca(u) < d_time(v))
                    hca(v) = min(hca(v), hca(u));
                else
                {
                    while (v != S.pop())
                        S.push(v);
                }
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD;
                if (u! = parent(v))
                    hca(v) = min(hca(v), d_time(u));
                break;
            default:
                type(v, u) = (d_time(v) < d_time(u)) ? FORWARD : CROSS;
                break;
        }
    }
    status(v) = VISITED;
}
#undef hca

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater)
{
    _reset();
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
            _pfs(v, prioUpdater);
    } while (s != (v = (++v % n)));
}

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::_pfs(int s, PU prioUpdater)
{
    priority(s) = 0;
    status(s) = VISITED;
    parent(s) = -1;
    while (true)
    {
        for (int w = first_nbr(s); -1 < w; w = next_nbr(s, w))
            prioUpdater(this, s, w);
        for (int shortest = INT_MAX, w = 0; w < n; w++)
        {
            if (UNDISCOVERED == status(w))
            {
                if (shortest > priority(w))
                {
                    shortest = priority(w);
                    s = w;
                }
            }
        }
        if (VISITED == status(s))
            break;
        stauts(s) = VISITED;
        type(parent(s), s) = TREE;
    }
}

template<typename Tv, typename Te>
void PrimPU::operator()(Graph<Tv, Te> * g, int uk, int v)
{
    if (UNDISCOVERED == g->status(v))
    {
        if (g->priority(v) > g->weight(uk, v))
        {
            g->priority(v) = g->weight(uk, v);
            g->parent(v) = uk;
        }
    }
}

template<typename Tv, typename Te>
void DijkstraPU::operator()(Graph<Tv, Te> * g, int uk, int v)
{
    if (UNDISCOVERED == g->status(v))
    {
        if (g->priority(v) > g->priority(uk) + g->weight(uk, v))
        {
            g->priority(v) = g->priority(uk) + g->weight(uk, v);
            g->parent(v) = uk;
        }
    }
}