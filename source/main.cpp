#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVertexGlyphFilter.h>

#include <sstream>
#include <iostream>
#include "Octree.h"


int main(int argc, char* argv[]) {
    int granularidad = 1;
    int x_min = 0;
    int y_min = 0;
    int longitudLado = 250;


    OcTree arbol(x_min, y_min, longitudLado, granularidad);
    //arbol.PrintEnds();

    arbol.insert(130, 130, arbol);
    arbol.insert(100, 50, arbol);
    arbol.insert(30, 110, arbol);
    arbol.insert(170,40, arbol);
    arbol.insert(200,200, arbol);
    arbol.insert(50,80, arbol);

    //arbol.printOctree();

    return 0;
}