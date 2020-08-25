/*
Estructura de Datos 2017/2018 - Practica Final "Quien es Quien"
Nombre: Ramon Sanchez Garcia
Correo: chentaco@correo.ugr.es
*/
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

#include "../include/quienesquien.h"

using namespace std;

int main(int argc, char * argv[]){

	bool jugar = false;
	bool limpiar = false;
	QuienEsQuien quienEsQuien;
	QuienEsQuien mejorado;
	char version, tecla;

	if(argc == 2){
		string parametroAleatorio = "aleatorio";
		if(argv[1]== parametroAleatorio){
			cout << "Creando un QuienEsQuien aleatorio"<< endl;
			int numero_de_personajes;
		   	int numero_de_atributos;

		   	do{
		   		cout << "¿Número de personajes? ";
		   		cin >>numero_de_personajes;
		   	}while(numero_de_personajes<=0);

		   	quienEsQuien.tablero_aleatorio(numero_de_personajes);
			quienEsQuien.mostrar_estructuras_leidas();

		   	string nombreFicheroSalida = string("datos/tablero") +
		   		"-num-per="+to_string(numero_de_personajes)+".csv";

		   	cout << "Guardando tablero aleatorio en "<<nombreFicheroSalida<<endl;
		   	ofstream archivoDeSalida(nombreFicheroSalida);
		   	archivoDeSalida << quienEsQuien;

		   	cout << "Guardado"<<endl;
		   	return 0;

		}else{
			cout << "Cargando fichero para jugar'"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
			f>> quienEsQuien;
			mejorado = quienEsQuien;
			jugar = true;
		}
	} else if(argc == 3 ){
		string parametroLimpiar = "limpiar";
		if(parametroLimpiar== argv[2]){
			cout << "Cargando fichero para limpiar (sin jugar) '"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
			f>> quienEsQuien;
			mejorado = quienEsQuien;
			jugar = false;
		} else{
			cout << "Modo '"<<argv[2]<<"' no reconocido"<<endl;
			return 1;
		}

	} else {
		cout << "No se reconocen los argumentos. Ejemplos de uso:" << endl;
		cout << "\tJugar:               ./bin/quienesquien RUTA_FICHERO"<< endl;
		cout << "\tLimpiar sin jugar:   ./bin/quienesquien RUTA_FICHERO limpiar"<< endl;
		cout << "\tGenerar aleatorio:   ./bin/quienesquien aleatorio"<< endl;
		return 1;
	}

	while(version!='1' && version!='2'){
		cout << "Que version quieres usar? Normal(1)/Mejorada(2) || NOTA: La mejora se nota mas en tableros grandes." << endl;
		cin >> version;
		if(version!='1' && version!='2'){
			cout << "OPCION INCORRECTA" << endl;
		}
	}

	if(version=='1'){
		cout << "ESTE ES EL ASPECTO DEL FICHERO LEIDO: " << endl;
		quienEsQuien.mostrar_estructuras_leidas();
		quienEsQuien.usar_arbol(quienEsQuien.crear_arbol());
		cout << "=========== Arbol en crudo ===========" << endl;
		quienEsQuien.escribir_arbol_completo();
		cout << "Profundidad promedio de las hojas del arbol: ";
		cout << quienEsQuien.profundidad_promedio_hojas() << endl;
		cout << "======================================" << endl << endl << endl;
		quienEsQuien.eliminar_nodos_redundantes();
		cout << "=========== Arbol ===================="<<endl;
		quienEsQuien.escribir_arbol_completo();
		cout << "Profundidad promedio de las hojas del arbol: ";
		cout << quienEsQuien.profundidad_promedio_hojas()<<endl;
		cout << "======================================" << endl << endl << endl;

		if(jugar){
			quienEsQuien.iniciar_juego();
		}

		cout << "A continuacion se muestra un ejemplo de insercion y eliminacion de personaje." << endl;
		cout << "Se va a insertar Pepe con el vector [1,1,1] y eliminar el jugador Personaje 25." << endl;
		cout << "PULSA UNA TECLA y DESPUES ENTER PARA CONTINUAR." << endl;
		cin >> tecla;

		// SOLO FUNCIONAN CON EL QUIENESQUIEN NORMAL, NO CON EL OPTIMIZADO
		// LA PRUEBA DE A CONTINUACION ESTA IDEADA PARA EL ARBOL DE 3X6
		// se puede hacer manualmente, pero por falta de tiempo lo dejo asi
		vector<bool> prueba;
		prueba.push_back(1);
		prueba.push_back(1);
		prueba.push_back(1);
		cout << "=========== EJEMPLO INSERCION ===================="<<endl;
		string personaje_a = "Pepe";
		cout << "Insertando el personaje: " << personaje_a << endl;
		quienEsQuien.insertar_jugador(personaje_a,prueba);
		quienEsQuien.escribir_arbol_completo();
		// Ademas muestro el tablero
		quienEsQuien.mostrar_estructuras_leidas();
		cout << "=========== EJEMPLO ELIMINACION ===================="<<endl;
		string personaje_b = "Personaje_25";
		cout << "Borrando el personaje: " << personaje_b << endl;
		quienEsQuien.eliminar_jugador(personaje_b);
		quienEsQuien.escribir_arbol_completo();
		// Ademas muestro el tablero
		quienEsQuien.mostrar_estructuras_leidas();
	}else if(version=='2'){
		// Uso del arbol con la mejora implementada
		mejorado.mostrar_estructuras_leidas();
		mejorado.usar_arbol(mejorado.crear_arbol_mejorado());
		cout << "=========== Arbol en crudo ===========" << endl;
		mejorado.escribir_arbol_completo();
		cout << "Profundidad promedio de las hojas del arbol: ";
		cout << mejorado.profundidad_promedio_hojas() << endl;
		cout << "======================================" << endl << endl << endl;
		mejorado.eliminar_nodos_redundantes();
		cout << "=========== Arbol ===================="<<endl;
		mejorado.escribir_arbol_completo();
		cout << "Profundidad promedio de las hojas del arbol: ";
		cout << mejorado.profundidad_promedio_hojas()<<endl;
		cout << "======================================" << endl << endl << endl;

		if(jugar){
			mejorado.iniciar_juego();
		}
	}

	return 0;
}
