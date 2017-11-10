#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <set>
#include <algorithm>

using namespace std;






class Point{
public:
    vector<double> data;
    double reach_dist;
    bool isProcessed;
    
    Point(){
        data = vector<double>();
        reach_dist = numeric_limits<double>::max();
        isProcessed = false;
    }
};







//euclidean distance btwn two points
double euclideanDist(Point* p1,Point* p2){
    
    double euDist=0;
    double mag=0;
    
    for(int i=0;i<p1->data.size();i++){
        mag = p1->data[i]-p2->data[i];
        euDist+=(mag*mag);
    }
    
    return euDist;
}








//computing core distance
Point* p1 = new Point();

bool customComp2(Point* a,Point* b){
    //cout<<p1->data[0]<<" ";
    return (euclideanDist(p1,a) < euclideanDist(p1,b));
}

double coreDistance(Point* p,int minPts,vector<Point*>& neigbh) {

    double coreDist = numeric_limits<double>::max();
    p1 = p;
    if(neigbh.size()>=minPts){
        sort(neigbh.begin(),neigbh.end(),customComp2);
        coreDist = euclideanDist(p, neigbh[minPts-1]);
    }
    
    return coreDist;
}







//finding neigbours within eps
vector<Point*> neigbhours(Point* p1,double eps,vector<Point*>& setOfPoints) {
    
    vector<Point*> NeighB;
    double epss = eps*eps;
    
    for(auto it=setOfPoints.begin();it!= setOfPoints.end();it++){
        Point* p2 = *it;
        if(euclideanDist(p1,p2) <= epss){
            NeighB.push_back(p2);
        }
    }
    return NeighB;
    
}








//override the insert operator for orderedSeeds set
class customComparator {
public:
    bool operator() (Point* p1,Point* p2){
        
        if(p1->reach_dist<p2->reach_dist || (p1->reach_dist==p2->reach_dist && p1<p2))
            return true;
        
        return false;
    }
    
};


//updating orderedSeed
void update(vector<Point*>& neigbh,Point* center_object,double coreDist, set<Point*, customComparator>& orderedSeeds) {
    
    for(auto it=neigbh.begin();it!=neigbh.end();it++) {
        Point* object = *it;
        
        if(!object->isProcessed){
            double reachDist_new = max(coreDist,euclideanDist(center_object,object));
            
            if(object->reach_dist == numeric_limits<double>::max()){
                object->reach_dist =  reachDist_new;
                orderedSeeds.insert(object);
            }
            else if(reachDist_new < object->reach_dist){
                //o already in OrderSeeds
                orderedSeeds.erase(object);object->reach_dist = reachDist_new;
                orderedSeeds.insert(object);
            }
        }
        
    }
}











//expanding cluster ordering for unprocessed point
void expandCluster(vector<Point*>& setOfPoints,Point* o,double eps,int minPts,vector<Point*>& orderedPoints){
    
    vector<Point*> neigbh = neigbhours(o,eps,setOfPoints);
    o->isProcessed = true;
    o->reach_dist = numeric_limits<double>::max();
    
    double coreDist1 = coreDistance(o, minPts, neigbh);
    orderedPoints.push_back(o);
    
    if(coreDist1!=numeric_limits<double>::max()){
        set<Point*,customComparator> orderedSeeds;
        update(neigbh,o,coreDist1,orderedSeeds);
        
        while(!orderedSeeds.empty()) {
            Point* o2 = *orderedSeeds.begin();orderedSeeds.erase(orderedSeeds.begin());
            
            vector<Point*> neigbh2 = neigbhours(o2,eps,setOfPoints);
            o2->isProcessed = true;
            double coreDist2 = coreDistance(o2,minPts,neigbh2);
            orderedPoints.push_back(o2);
            
            if( coreDist2 != numeric_limits<double>::max()) {
                update(neigbh2,o2,coreDist2,orderedSeeds);
            }
        }

    }
    
}












int main(int argc, const char * argv[]) {
    
    string input_file,eps_string,minPts_string;
    input_file = "data.tsv";
    
    if(argc<3){
        cout<<"Error: Please Provide values for minPts and eps";
        return 0;
    }
    
    minPts_string = argv[1];
    eps_string = argv[2];
    
    float eps = stof(eps_string);
    int minPts = stoi(minPts_string);
    
    
    ifstream infile;
    infile.open(input_file);
    
    string str;
    float doub;
    
    vector<Point*> setOfPoints;
    
    //reading input file
    while(getline(infile,str)){
        istringstream iss(str);
        Point* tmp = new Point();
        while(iss>>doub){
            tmp->data.push_back(doub);
            //cout<<doub<<" ";
        }
        //cout<<endl;
        setOfPoints.push_back(tmp);
    }
    
    //OPTICS clustering
    vector<Point*> orderedPoints;
    for(auto it = setOfPoints.begin();it!=setOfPoints.end();it++){
        Point* o = *it;
        
        if(!o->isProcessed)
            expandCluster(setOfPoints,o,eps,minPts,orderedPoints);
    }
    cout<<orderedPoints.size()<<endl;
    
    
    
    return 0;
}
