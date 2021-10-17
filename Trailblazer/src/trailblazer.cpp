/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement TrailBlazer
 */


#include "trailblazer.h"
#include "Color.h"
#include "pqueue.h"
#include "queue.h"
#include "hashset.h"
using namespace std;
bool DFSHelper(BasicGraph& graph, Vertex* current, Vertex* end, Vector<Vertex*>& path);
void reconstructPath(Vertex* end, Vector<Vertex*>& path);
struct myCost{
public:
    myCost(string name){
        this->algo = name;
    }
    double operator()(string algo, Vertex* current, Vertex* end){
        if(algo =="dijkstrasAlgorithm") return current->cost;
        else if(algo =="aStar") return heuristicFunction(current,end);
        else throw string("this algorothm is not implimented or does not exists");
    }

    string algo;

};
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();
    path += start;
    bool found = DFSHelper(graph, start, end, path);
    if(found) return path;
    else throw string("no path what found");
}
bool DFSHelper(BasicGraph& graph, Vertex* current, Vertex* end, Vector<Vertex*>& path){
    //base case
    if(current->name == end->name) return true;
    Set<Vertex*> neighs = graph.getNeighbors(current);
    current->visited = true;
    current->setColor(GREEN);
    for(Vertex* neigh : neighs){
        if(!neigh->visited) neigh->setColor(YELLOW);
    }
    for(Vertex* neigh : neighs){
         //choose
        if(neigh->visited) continue;
         path += neigh;
         neigh->visited = true;
         neigh->setColor(GREEN);
         current = neigh;
         //explore
         if(DFSHelper(graph, current, end, path)) return true;
         //unchoose
         path.remove(path.size()-1);
         neigh->setColor(GRAY);
    }
    return false;
}

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    graph.resetData();

    Queue<Vertex*> pq;
    pq.enqueue(start);
    start->visited = true;
    while(!pq.isEmpty()){
        Vertex* current = pq.dequeue();
        current->setColor(GREEN);
        if(current->name == end->name) break;
        Set<Vertex*> neighs = graph.getNeighbors(current);
        for(Vertex* neigh : neighs){
            if(!neigh->visited){
                neigh->visited = true;
                neigh->setColor(YELLOW);
                neigh->previous = current;
                pq.enqueue(neigh);
            }
        }
    }
    reconstructPath(end, path);
    return path;
}

Vector<Vertex*> genericAlgo(BasicGraph& graph, Vertex* start, Vertex* end, myCost funct) {
    Vector<Vertex*> path;
    graph.resetData();
    PriorityQueue<Vertex*> pq;
    Set<Vertex*> allVertex = graph.getVertexSet();
    for(Vertex* vtx : allVertex){
        vtx->cost = POSITIVE_INFINITY;
        vtx->previous = NULL;
    }
    start->cost = 0.;
    pq.enqueue(start, start->cost);
    while (!pq.isEmpty()) {
        Vertex* current = pq.dequeue();
        current->visited = true;
        current->setColor(GREEN);
        if(current->name == end->name) break;
        Set<Vertex*> neighs = graph.getNeighbors(current);
        for(Vertex* neigh : neighs){
            if(!neigh->visited){
                neigh->setColor(YELLOW);
                Edge* currentEdge = graph.getArc(current, neigh);
                double tempCost = current->cost + currentEdge->cost;
                if(tempCost < neigh->cost){
                    neigh->cost = tempCost;
                    neigh->previous = current;
                    pq.enqueue(neigh, funct(funct.algo, neigh, end));
                }
            }
        }
    }
    reconstructPath(end, path);
    return path;
}


Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    myCost funct("dijkstrasAlgorithm");
    return genericAlgo(graph, start, end, funct);
}
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    myCost funct("aStar");
    return genericAlgo(graph, start, end, funct);
}

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Set<Edge*> edges = graph.getEdgeSet();
    Set<Vertex*> vts = graph.getVertexSet();
    PriorityQueue<Edge*> pqEdges;
    HashSet<HashSet<Vertex*>* > clusters;

    for(Edge* e : edges) pqEdges.enqueue(e, e->cost);
    for(Vertex* v : vts) {
        //asicGraph* temp = new BasicGraph();
        HashSet<Vertex*>* temp = new HashSet<Vertex*>();
        temp->add(v);
        clusters.add(temp);
    }
    while(clusters.size() > 1){
        Edge* edge = pqEdges.dequeue();
        Vertex* start = edge->start;
        Vertex* end = edge->finish;
        HashSet<Vertex*>* fCluster = NULL;
        HashSet<Vertex*>* sCluster = NULL;
        for(HashSet<Vertex*>* hS : clusters){
            if(hS->contains(start)) fCluster = hS;
            else if(hS->contains(end)) sCluster = hS;
            else if(fCluster != NULL && sCluster != NULL) break;
        }
        if(fCluster != sCluster && fCluster && sCluster){
            mst.add(edge);
            if(fCluster->size() >= sCluster->size()){
                fCluster->addAll(*sCluster);
                clusters.remove(sCluster);
            }
            else {
                sCluster->addAll(*fCluster);
                clusters.remove(fCluster);
            }
        }
    }

    return mst;
}

void reconstructPath(Vertex* end, Vector<Vertex*>& path){
    Vector<Vertex*> temp;
    Vertex* previous = end;

    while(previous){
        temp += previous;
        previous = previous->previous;
    }
    for(int i = temp.size()-1; i >=0; i--){
        path += temp[i];
    }
}
