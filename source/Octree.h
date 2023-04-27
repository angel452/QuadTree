#include <iostream>
#include <vector>
using namespace std;

struct Point
{
    int valorX;
    int valorY;

    Point(){
        valorX = -1;
        valorY = -1;
    }

    Point(double _valorX, double _valorY) {
        valorX = _valorX;
        valorY = _valorY;
    }
};

class OcTree{
    private:

        Point *extremoMin;
        int longitudLado; //Point *extremoMax;
        
        int maxNumPoints; // GRANULARIDAD: el numero maximo de puntos que soportara un cubo

        OcTree *hijos[4]; // Cada uno de sus 8 hijos seran de tipo Octree
        Point *puntos;
        int nPoints; // numero de puntos insertados
        int profundidad;
        bool isLeaf;
        bool haveChildrens;


    public:

        vector< vector<int> > allSizes; // Se guardaran todos puntos para hacer el cuadrante

        // Constructor para los hijos
        OcTree(){
            puntos = new Point();
        }

        // MAIN CONSTRUCTOR
        OcTree(int xMin, int yMin, int _longitudLado,int granularidad){

            maxNumPoints = granularidad;
            extremoMin = new Point(xMin, yMin);
            longitudLado = _longitudLado;

            puntos = new Point();

            //nPoints = _nPoints;
            nPoints = 0;
            profundidad = 0;
            isLeaf = true;
            haveChildrens = false;
        }

        void PrintEnds(){
            cout << "-------------------------------------" << endl;
            cout << "Minimos:" << endl;
            cout << "X:" << extremoMin->valorX << " y Y: " << extremoMin->valorY << endl;
            cout << "Maximos:" << endl;
            cout << "X:" << longitudLado << " y Y: " << longitudLado << endl;
            cout << "-------------------------------------" << endl;
        }

        void splitOctree(OcTree &nodoActual){

            int longitudLadoAux = nodoActual.longitudLado/2;

            int x_min0 = nodoActual.extremoMin->valorX;
            int y_min0 = nodoActual.extremoMin->valorY + nodoActual.longitudLado/2;  
            nodoActual.hijos[0] = new OcTree(x_min0, y_min0, longitudLadoAux, maxNumPoints);

            int x_min1 = nodoActual.extremoMin->valorX + nodoActual.longitudLado/2;
            int y_min1 = nodoActual.extremoMin->valorY + nodoActual.longitudLado/2;  
            nodoActual.hijos[1] = new OcTree(x_min1, y_min1, longitudLadoAux, maxNumPoints);

            int x_min2 = nodoActual.extremoMin->valorX;
            int y_min2 = nodoActual.extremoMin->valorY;  
            nodoActual.hijos[2] = new OcTree(x_min2, y_min2, longitudLadoAux, maxNumPoints);

            int x_min3 = nodoActual.extremoMin->valorX + nodoActual.longitudLado/2;
            int y_min3 = nodoActual.extremoMin->valorY;         
            nodoActual.hijos[3] = new OcTree(x_min3, y_min3, longitudLadoAux, maxNumPoints);

            cout << "** Divicion completada **" << endl;
        }

        bool isIn(OcTree &nodoActual, Point puntos,int pos){
            int extremoX = nodoActual.hijos[pos]->extremoMin->valorX;
            int extremoY = nodoActual.hijos[pos]->extremoMin->valorY;
            int longitud = nodoActual.hijos[pos]->longitudLado;

            int pntoX = puntos.valorX;
            int pntoY = puntos.valorY;
       
            if( pntoX > extremoX && pntoX < extremoX+longitud ){
                if( pntoY > extremoY && pntoY < extremoY+longitud ){
                    return true;
                }
            }
            return false;
        }

        int chooseGroup(OcTree &nodoActual, Point puntos){
            cout << "Eligiendo para: " << puntos.valorX << " - " << puntos.valorY << endl;
            for(int i = 0; i < 4; i++){ // Recorre los 4 hijos
                cout << "Analizando cuadrante " << i << endl;
                if( isIn(nodoActual, puntos, i) ){ // Pregunta si el nodo pertenece al grupo i
                    cout << "Entra en el cuadrado " << i << endl;
                    return i;
                }
            }
            //return -1;
        }

        void insert(int xPoint, int yPoint, OcTree &nodoActual){
            
            cout << "Insertando: " << xPoint << " - " << yPoint << endl;
            cout << "NPuntos(Antes): " << nodoActual.nPoints << endl;

            // solo entra si el nodo actual es una hoja y si NO supera el maximo de puntos
            if (nodoActual.isLeaf == true && nodoActual.nPoints < maxNumPoints){
                cout << "Es hoja + limite aceptado" << endl;

                Point *aux = new Point(xPoint, yPoint);
                nodoActual.puntos[nodoActual.nPoints].valorX = aux->valorX;
                nodoActual.puntos[nodoActual.nPoints].valorY = aux->valorY;
                cout << "**Dato insertado **" << endl;

                nodoActual.nPoints++;
                cout << "NPuntos(Ahora): " << nodoActual.nPoints << endl;
            }
            else{
                cout << "Supera granularidad" << endl;
                if(nodoActual.isLeaf == true){ // si es una hoja -> DIVIDE y reinserta
                    cout << " ** Es una hoja - Dividiendo **  " << endl;

                    splitOctree(nodoActual);
                    nodoActual.isLeaf = false;
                    nodoActual.haveChildrens = true;
                    
                    cout << "------------ Cuadrantes -------------" << endl;
                    for(int i = 0; i < 4; i++){
                        cout << "Cuadrante " << i << " -> " << nodoActual.hijos[i]->extremoMin->valorX << " - " << nodoActual.hijos[i]->extremoMin->valorY << endl;
                    }
                    cout << "Con Longitud:" << (nodoActual.longitudLado)/2 << endl;
                    cout << "-------------------------------------" << endl;

                    // ###################### REINSERTAR NODOS #################################
                    cout << "Insertando Temporalmente al nuevo dato 1" << endl; // es el C
                    Point *aux = new Point(xPoint, yPoint);
                    nodoActual.puntos[nodoActual.nPoints].valorX = aux->valorX;
                    nodoActual.puntos[nodoActual.nPoints].valorY = aux->valorY;
                    
                    nodoActual.nPoints++;
                    cout << "NPuntos(Ahora): " << nodoActual.nPoints << endl;

                    for(int i = 0; i < nodoActual.nPoints; i++){
                        cout << "** ReInsert " << i+1 << " **" << endl;
                        //cout << nodoActual.puntos[i].valorX << " - " << nodoActual.puntos[i].valorY << endl;

                        int group = chooseGroup( nodoActual, nodoActual.puntos[i]);

                        insert(nodoActual.puntos[i].valorX, nodoActual.puntos[i].valorY, *nodoActual.hijos[group] ); 
                        //sleep(1);
                    }

                    // Eliminar Nodos ya re insertados:
                    cout << "------ para eliminar 1-----------" << endl;
                    cout << "Actualmente hay: " << nodoActual.nPoints <<  " puntos"<< endl; 
                    int auxDelete = nodoActual.nPoints;
                    for(int i = 0; i < auxDelete ; i++){
                        nodoActual.nPoints = nodoActual.nPoints - 1;
                        //delete
                    }
                    cout << "naw: " << nodoActual.nPoints <<  " puntos"<< endl; 

                }
                else{ // Si no es una hoja -> recorre sus hijos
                    cout << " ** Recorriendo hojas **" << endl;
                    cout << "Insertando Temporalmente al nuevo dato 2" << endl;
                    Point *aux = new Point(xPoint, yPoint);
                    nodoActual.puntos[nodoActual.nPoints].valorX = aux->valorX;
                    nodoActual.puntos[nodoActual.nPoints].valorY = aux->valorY;

                    nodoActual.nPoints++;
                    cout << "NPuntos(Ahora): " << nodoActual.nPoints << endl;
                    
                    cout << "Viendo a que hijo ira el dato temporal" << endl;
                    int group = chooseGroup(nodoActual, nodoActual.puntos[nodoActual.nPoints-1]);
                    cout << "El nuevo dato pertenece al cuadrante " << group <<  endl;

                    insert(nodoActual.puntos[nodoActual.nPoints-1].valorX, nodoActual.puntos[nodoActual.nPoints-1].valorY, *nodoActual.hijos[group]);
            
                    cout << "------ para eliminar 2-----------" << endl;
                    cout << "Actualmente hay: " << nodoActual.nPoints <<  " puntos"<< endl; 
                    int auxDelete = nodoActual.nPoints;
                    for(int i = 0; i < auxDelete ; i++){
                        nodoActual.nPoints = nodoActual.nPoints - 1;
                        //delete
                    }
                    cout << "naw: " << nodoActual.nPoints <<  " puntos"<< endl; 
                }   
            }
            cout << "-------------------------------------" << endl;
        }

        void getMinimunSizes(OcTree nodoActual, int i){
            //cout << "Verificando hijo: "<< i << endl;
            if(nodoActual.haveChildrens == false){
                //cout << "NOOOO tiene hijos" << endl;
                return;
            }
            else{
                //cout << "SII tiene hijos" << endl;

                for(int z = 0; z < 4; z++){
                    vector<int> aux;
                    aux.push_back( nodoActual.hijos[z]->extremoMin->valorX );
                    aux.push_back( nodoActual.hijos[z]->extremoMin->valorY );
                    aux.push_back( nodoActual.hijos[z]->longitudLado );

                    //cout << "x:" << nodoActual.hijos[z]->extremoMin->valorX << " y: " << nodoActual.hijos[z]->extremoMin->valorY << " len: " << nodoActual.hijos[z]->longitudLado << endl;

                    allSizes.push_back(aux);
                }               

                for(int i = 0; i < 4; i++){
                    getMinimunSizes(*nodoActual.hijos[i], i);
                }
            }
        }

        void printOctree(){
            cout << "Nivel 0" << endl;
            for(int i = 0; i < 5; i++){
                cout << puntos[i].valorX << " - " << puntos[i].valorY << endl;
            }

            cout << "Nivel 1" << endl;
            for(int i = 0; i < 4; i++){
                cout << hijos[i]->puntos->valorX << " - " << hijos[i]->puntos->valorY << endl;
            }
        }
};