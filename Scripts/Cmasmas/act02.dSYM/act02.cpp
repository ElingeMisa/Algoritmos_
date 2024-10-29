//  Victoria Valentina Marin Dominguez A00836008
//  act02.cpp
//  act02
//
//  Created by Victoria Marin on 10/08/24.
//

#include <iostream>
#include <string>
using namespace std;

int main() {
    
    //Inicializaciones O(1)
    int lengthN;
    string chain;
    
    //Input de la longitud del string y del string
    cin >> lengthN;
    cin >> chain; //O(n)
    
    //Contadores de ceros y unos O(1)
    int ceros = 0, unos = 0;

    //Pasa por el string y cuenta los ceros y unos
    //For O(n)
    
    for (int i = 0; i < chain.length(); i++)
    {
        if (chain[i] == '0') ceros++;
        else if (chain[i] == '1') unos++;
    }

    /*
    for (auto c : chain) {
        //If O(1)
        if (c == '0') ceros++;
        else if (c == '1') unos++;
    }
    */
        
    //O(1)
    //La longitud minima es el valor absoluto de la diferencia entre los 0s y 1s
    int result = abs(ceros - unos);

    // Imprimir el resultado
    //O(1)
    cout << result << endl;

    return 0;
        
}

