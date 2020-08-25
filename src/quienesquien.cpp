/*
Estructura de Datos 2017/2018 - Practica Final "Quien es Quien"
Nombre: Ramon Sanchez Garcia
Correo: chentaco@correo.ugr.es
*/


#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>

QuienEsQuien::QuienEsQuien(){

}
QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
	this->personajes=quienEsQuien.personajes;
	this->atributos=quienEsQuien.atributos;
	this->tablero=quienEsQuien.tablero;
	this->arbol=quienEsQuien.arbol;
	this->jugada_actual=quienEsQuien.jugada_actual;
}
QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
	this->personajes=quienEsQuien.personajes;
	this->atributos=quienEsQuien.atributos;
	this->tablero=quienEsQuien.tablero;
	this->arbol=quienEsQuien.arbol;
	this->jugada_actual=quienEsQuien.jugada_actual;
	return *this;
}
QuienEsQuien::~QuienEsQuien(){
	this->limpiar();
}

void QuienEsQuien::limpiar(){
	personajes.clear();
	atributos.clear();
	tablero.clear();
	arbol.clear();
	//jugada_actual=0;
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
	cout << "personajes: "<< (this->personajes) << endl;
	cout << "atributos:  "<< (this->atributos)  << endl;
	cout << "tablero:    "<< endl;


	// Escribe la cabecera del tablero
	for(vector<string>::iterator it_atributos = this->atributos.begin();
		it_atributos != this->atributos.end();
		it_atributos++){
		cout << *it_atributos << "\t";
	}
	cout <<endl;

	int indice_personaje = 0;
	for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
		it_tablero_atributos!= tablero.end();
		it_tablero_atributos++){

		string personaje = this->personajes[indice_personaje];
		int indice_atributo = 0;
		for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
			it_atributos_personaje != (*it_tablero_atributos).end();
			it_atributos_personaje++){

			cout << *it_atributos_personaje<<"\t";

			indice_atributo++;
		}

		cout << personaje << endl;

		indice_personaje++;
	}

}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */
string limpiar_string(string cadena_original,string cadena_a_eliminar){
	string linea(cadena_original);

	while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
		linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
	}

	return linea;
}

istream& operator >> (istream& is, QuienEsQuien &quienEsQuien) {
	quienEsQuien.limpiar();

  	if(is.good()){
  		string linea;
    	getline(is, linea, '\n');

    	linea = limpiar_string(linea,"\r");

    	while(linea.find('\t') != string::npos ){
    		string atributo = linea.substr(0,linea.find('\t'));
			quienEsQuien.atributos.push_back(atributo);
    		linea = linea.erase(0,linea.find('\t')+1);
    	}

		assert(linea ==  "Nombre personaje");
	}

    while( is.good() ) {
    	string linea;
    	getline(is, linea, '\n');
    	linea = limpiar_string(linea,"\r");

    	//Si la linea contiene algo extrae el personaje. Si no lo es, la ignora.
    	if(linea != ""){;
	    	vector<bool> atributos_personaje;

	    	int indice_atributo=0;
	    	while(linea.find('\t') != string::npos){
	    		string valor = linea.substr(0,linea.find('\t'));

	    		assert(valor == "0" || valor == "1");

	    		bool valor_atributo = valor == "1";

	    		atributos_personaje.push_back(valor_atributo);

	    		linea = linea.erase(0,linea.find('\t')+1);
	    		indice_atributo++;
			}

			string nombre_personaje = linea;

	    	quienEsQuien.personajes.push_back(nombre_personaje);
	    	quienEsQuien.tablero.push_back(atributos_personaje);
	    }
  	}

	return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){

	//Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre
	for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
		it_atributos != quienEsQuien.atributos.end();
		it_atributos++){

		os  << *it_atributos << "\t";
	}
	os << "Nombre personaje" << endl;

	//Rellenamos con ceros y unos cada l�nea y al final ponemos el nombre del personaje.
	for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
		for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){

			os  << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
		}
		os  << quienEsQuien.personajes[indice_personaje] << endl;
	}

	return os;
}

/**
  * @brief Convierte un n�mero a un vector de bool que corresponde
  *        con su representaci�n en binario con un numero de digitos
  *        fijo.
  *
  * @param n N�mero a convertir en binario.
  * @param digitos N�mero de d�gitos de la representaci�n binaria.
  *
  * @return Vector de booleanos con la representaci�n en binario de @e n
  *      con el n�mero de elementos especificado por @e digitos.

  */
vector<bool> convertir_a_vector_bool(int n, int digitos) {
  vector<bool> ret;
  while(n) {
    if (n&1){
      ret.push_back(true);
    } else{
      ret.push_back(false);
    }
    n>>=1;
  }

  while(ret.size()<digitos){
  	ret.push_back(false);
  }

  reverse(ret.begin(),ret.end());
  return ret;
}

bintree<Pregunta> QuienEsQuien::crear_arbol(){
	// Nodo Raiz
	int numel=personajes.size();
	Pregunta p(atributos[0],numel);
	//cout << p << endl;
	// Creo el arbol
	bintree<Pregunta> arbol(p);

	string prueba;

	int indice_personaje = 0;
	for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
		it_tablero_atributos!= tablero.end();
		it_tablero_atributos++){

		// it_tablero_atributos hace referencia a los primeros elemntos de la filas

		//jugada_actual = arbol.root();
		bintree<Pregunta>::node mi_nodo = arbol.root();

		vector<vector<bool>> tablero_aux = tablero;

		int indice_atributo = 1;
		//int iguales=0;

		for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
			it_atributos_personaje != (*it_tablero_atributos).end();
			it_atributos_personaje++){

			// Cuando llegue a un nodo hoja, que aniada en lugar de un atributo, el nombre del personaje

			if (indice_atributo < atributos.size()){
				prueba=atributos[indice_atributo];
			}else{
				prueba=personajes[indice_personaje];
			}

			// Para introducir el numero de jugadores que responden a la pregunta, he
			// hecho lo siguiente: comparo cuantos atributos tienen iguales, dependiendo
			// del atributo, o atributos. Despues aumento el contador de iguales.
			// Siempre comparo dicho atribuo de jugador con el propio y el resto.
			// Dada la implementacion, siempre se quedara con el mayor numero. Al hacer
			// el siguiente recorrido, que tiene un numero menor, no podra meterlo al
			// arbol, ya que se metio uno anteriormente

			vector<vector<bool>>::iterator busqueda2 = it_tablero_atributos;
			int iguales=0;
			while(busqueda2!=tablero.end()){

			//busqueda2++;
			//if(busqueda2!=tablero.end()){
				int diferencia = it_atributos_personaje - (*it_tablero_atributos).begin() + 1;

				//it_busqueda = search((*it_tablero_atributos).begin(), it_atributos_personaje,
				//(*busqueda2).begin(), (*busqueda2).begin()+diferencia);

				vector<bool> prueba_v((*it_tablero_atributos).begin(), it_atributos_personaje+1);
				vector<bool> prueba2_v((*busqueda2).begin(), (*busqueda2).begin()+diferencia);

				/*
				cout << "ESTE ES MI VECTOR: " << prueba_v << endl;
				cout << "Y ESTE ES EL OTRO: " << prueba2_v << endl;
				cout << "Y LA DIFERENCIA ES: " << diferencia << endl;
				*/
				if(prueba_v==prueba2_v){
					//cout << "SON IGUALES" << endl;
					iguales++;
					//cout << "TOTAL IGUALES: " << iguales << endl;
				}//else{
					//cout << "NO LO SON" << endl;
				//}
				busqueda2++;
			}

			// La propia implementacion del arbol provoca que cuando se obtenga un nuevo nodo pregunta pero con
			// diferentes numeros de personajes, no podra insertarlo, porque solo insertara los hijos que no esten a
			// null, y dichos hijos siempre se insertan la primera vez, que es cuando se hace la comprobacion, asi que
			// no hay ningun error

			Pregunta p2(prueba,iguales);

			// Voy a crear el arbol dependiendo del valor. Para evitar que me rescriba los nodos, solo escribire sobre nodos nulos

			if(*it_atributos_personaje==0){
				if(mi_nodo.left().null()) arbol.insert_left(mi_nodo,p2);
					mi_nodo = mi_nodo.left();
			}
			if(*it_atributos_personaje==1){
				if(mi_nodo.right().null()) arbol.insert_right(mi_nodo,p2);
					mi_nodo = mi_nodo.right();
			}
			//cout << *it_atributos_personaje<<"\t";

			indice_atributo++;
		}

		//cout << personaje << endl;

		indice_personaje++;
	}
	return arbol;
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
	arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego(){
	char opcion;
	set<string> en_pie;
	set<string>::iterator it;
	// mi jugada
	pair<string,string> jugada;
	// total de jugadas
	vector<pair<string,string>> total_jugadas;
	// y su Iteradores
	vector<pair<string,string>>::iterator it_j;
	cout << "Mi nombre es Monika. Vamos a jugar una partida~. Recuerda contestar si (Y) o no (N) por teclado: " << endl;
	jugada_actual = arbol.root();
	while(!jugada_actual.left().null() || !jugada_actual.right().null()){ // lo hago asi para saltarme un paso
		if(!total_jugadas.empty()){
			cout << "El personaje oculto tiene las siguientes caracteristicas:" << endl;
			for(it_j=total_jugadas.begin();it_j!=total_jugadas.end();++it_j){
				cout << (*it_j).first << " - " << (*it_j).second << endl;
			}
			cout << "pero aun no se cual es." << endl;
		}
		en_pie = informacion_jugada(jugada_actual);
		cout << "Los siguientes jugadores estan en pie: " << endl;
		cout << "[ ";
		for(it=en_pie.begin();it!=en_pie.end();it++){
			cout << (*it) << " ";
		}
		cout << "]" << endl;
		cout << (*jugada_actual) << endl;
		cin >> opcion;

		if(opcion=='N'){
			jugada = info_jugada(jugada_actual,0);
			total_jugadas.push_back(jugada);
			jugada_actual = jugada_actual.left();
		}
		else if(opcion=='Y'){
			jugada = info_jugada(jugada_actual,1);
			total_jugadas.push_back(jugada);
			jugada_actual = jugada_actual.right();
		}else{
			cout << "Lo siento, no reconozco tu opcion. Recuerda teclear Y para afirmaritvo y N para negativo." << endl;
		}
	}
	cout << "He adivinado tu personaje!~" << endl;
	cout << (*jugada_actual) << endl;
}

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){
	set<string> personajes_levantados;
	set<string> aux;
	string personaje;
	bintree<Pregunta>::node nodo;
	if(jugada_actual.left().null() && jugada_actual.right().null()){
		personajes_levantados.insert((*jugada_actual).obtener_personaje());
		return personajes_levantados;
	}else{
	if(!jugada_actual.left().null()){
		// Metos los de la izquierda
		nodo = jugada_actual.left();
		aux = informacion_jugada(nodo);
		personajes_levantados = union_jugadas(aux,personajes_levantados);
	}
	if(!jugada_actual.right().null()){
		// Meto los de las derecha
		nodo = jugada_actual.right();
		aux = informacion_jugada(nodo);
		personajes_levantados = union_jugadas(aux,personajes_levantados);
	}

		return personajes_levantados;
	}
}

// Metodo auxiliar para unir dos set

set<string> QuienEsQuien::union_jugadas(set<string> primero, set<string> segundo){
	set<string> solucion;
	solucion.insert(primero.begin(),primero.end());
	solucion.insert(segundo.begin(),segundo.end());
	return solucion;
}

void escribir_esquema_arbol(ostream& ss,
					  const bintree<Pregunta>& a,
		    		  bintree<Pregunta>::node n,
					  string& pre){
	if (n.null()){
	    ss << pre << "-- x" << endl;
	} else {
	    ss << pre << "-- " << (*n) << endl;
	    if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
			pre += "   |";
	    	escribir_esquema_arbol(ss,a, n.right(), pre);
	     	pre.replace(pre.size()-4, 4, "    ");
	      	escribir_esquema_arbol (ss,a, n.left(), pre);
	      	pre.erase(pre.size()-4, 4);
	    }
  	}
}

void QuienEsQuien::escribir_arbol_completo() const{
	string pre = "";
	escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}

void QuienEsQuien::eliminar_nodos_redundantes(){
	eliminar_nodos_redundantes(arbol.root());
}

void QuienEsQuien::eliminar_nodos_redundantes(bintree<Pregunta>::node n){
	// Voy a comprobar que el nodo no sea nulo
	if(!n.null()){
		if(ajuste_nodos_nulos(n)){
				// compruebo la raiz por si me dejo alguno
				eliminar_nodos_redundantes(arbol.root());
		}
		eliminar_nodos_redundantes(n.left());
		eliminar_nodos_redundantes(n.right());
	}
}

// Funcion auxiliar para comprobar que tiene hijos nulos

bool QuienEsQuien::tengo_hijos_nulos(bintree<Pregunta>::node n){
	if(n.left().null() || n.right().null())
		return true;
	return false;
}

// el siguiente nodo debe buscar desde la hoja
bool QuienEsQuien::ajuste_nodos_nulos(bintree<Pregunta>::node n){
    bool modificado=false; // el modificado
		bintree<Pregunta> otroArbol;
		// solo entrara si uno de los hijos es nulo
    if(tengo_hijos_nulos(n)){
			// si es root()
        if(n != arbol.root()){
            // Dependiendo de si entra a izquierda o derecha, sustituyo
            if(n.parent().left()==n){
                if(!(n.left().null()) && n.right().null()){
                    arbol.prune_left(n,otroArbol);
                    arbol.insert_left(n.parent(),otroArbol);
                    modificado=true;
                }else if(!(n.right().null()) && n.left().null()){
                    arbol.prune_right(n,otroArbol);
                    arbol.insert_left(n.parent(),otroArbol);
                    modificado=true;
                }
            }else if(n.parent().right()==n){
                if(!(n.left().null()) && n.right().null()){
                    arbol.prune_left(n,otroArbol);
                    arbol.insert_right(n.parent(),otroArbol);
                    modificado=true;
                }else if(!(n.right().null()) && n.left().null()){
                    arbol.prune_right(n,otroArbol);
                    arbol.insert_right(n.parent(),otroArbol);
                    modificado=true;
                }
            }
        }else{
            // En caso que sea nodo root
            if(!(n.left().null()) && n.right().null()){
                arbol.prune_left(n,otroArbol);
                arbol=otroArbol;
                modificado=true;
            }else if(!(n.right().null()) && n.left().null()){
                arbol.prune_right(n,otroArbol);
                arbol=otroArbol;
                modificado=true;
            }
        }
    }
    return modificado; // devolver si he cambiado o no
}

// Voy a calcular la profundidad promedio como una division entre el total de profundidades/total numero de nodos
float QuienEsQuien::profundidad_promedio_hojas(){

	int total = total_profundidad(arbol.root(),0);
	int n_nodos = contar(arbol.root());

	//cout << "LA DISTANCIA ES: " << total << endl;
	//cout << "EL NUMERO DE NODOS ES: "<< n_nodos << endl;

	return float(total)/float(n_nodos);
}

// Ya existe, pero es privado, voy a crearme uno propio mejor que editar los archivoDeSalida
// de bintree

int QuienEsQuien::contar(bintree<Pregunta>::node n){
  if (n.null())
    return 0;
  else
    return 1 + contar(n.left()) + contar(n.right());
}

int QuienEsQuien::total_profundidad(bintree<Pregunta>::node n, int acum){
	if(n.null()){
		return 0;
	}
	return acum + total_profundidad(n.left(), acum+1) + total_profundidad(n.right(), acum+1);
}

/////////////////////////////////////
/// METODOS ADICIONALES OPTATIVOS ///
/////////////////////////////////////

// Aunque el enunciado sugiere que reciba solo por parametro un nodo, yo Adicionalmente
// le paso el valor booleando de la opcion que he cogido

pair<string,string> QuienEsQuien::info_jugada(bintree<Pregunta>::node n, bool valor){
	pair<string,string> par_valores;
	if(valor==0)
		par_valores = pair<string,string>((*n).obtener_pregunta(),"no");
	else if(valor==1)
		par_valores = pair<string,string>((*n).obtener_pregunta(),"si");
	return par_valores;
}

// Funcion auxiliar que comprueba si un vector existe en el tablero
// Equivale a ver si un personaje ya existe con esos atributos

bool QuienEsQuien::esta_vector(vector<bool> prueba){
	vector<vector<bool>>::iterator p;
	p = find(tablero.begin(),tablero.end(),prueba);
	if(p!=tablero.end())
		return true;
	else
		return false;
}

void QuienEsQuien::insertar_jugador(string nombre, vector<bool> nuevo){
	bool esta = esta_vector(nuevo);
	if(!esta && nuevo.size()==atributos.size()){
		bintree<Pregunta>::node nodo = arbol.root();
		bintree<Pregunta>::node aux;
		bintree<Pregunta> otroArbol;
		vector<bool>::iterator it1 = nuevo.begin();
		vector<string>::iterator it2 = atributos.begin();
		int contador=0;
		while(!nodo.right().null() && !nodo.left().null()){
			//cout << (*nodo) << endl;
			if((*it1)==0){
				nodo = nodo.left();
			}
			else if((*it1)==1){
				nodo = nodo.right();
			}
			++it1;
			++it2;
		}
		aux = nodo; // guardo la hoja
		nodo = nodo.parent(); // vuelvo al padre

		Pregunta p_nueva((*it2),2);
		Pregunta nuevo_jugador(nombre,1);

		if (nodo.left()==aux){
			arbol.prune_left(nodo,otroArbol);
			arbol.insert_left(nodo,p_nueva);
			nodo = nodo.left();
		}else if(nodo.right()==aux){
			arbol.prune_right(nodo,otroArbol);
			arbol.insert_right(nodo,p_nueva);
			nodo = nodo.right();
		}
		if((*it1)==0){
			arbol.insert_left(nodo,nuevo_jugador);
			arbol.insert_right(nodo,*aux);
		}
		else if((*it1)==1){
			arbol.insert_left(nodo,*aux);
			arbol.insert_right(nodo,nuevo_jugador);
		}
		// La implementacion ya aumenta directamente el numero de personajes
		// Aniado el personaje y el vector a mi tablero
		this->tablero.push_back(nuevo);
		this->personajes.push_back(nombre);
	}else{
		cout << "EL PERSONAJE YA EXISTE O EL VECTOR NO ES DEL TAMANIO INDICADO" << endl;
	}
}

void QuienEsQuien::eliminar_jugador(string nombre){
	vector<bool> mi_vector;
	vector<string>::iterator it;
	int contador=0;
	bool encontrado = false;
	for(it=personajes.begin();it!=personajes.end() && !encontrado;++it){
		if((*it)==nombre){
			encontrado=true;
		}
		else{
			contador++;
		}
	}
	if(!encontrado){
		cout << "El personaje no ha sido encontrado" << endl;
	}else{
		mi_vector = (*(tablero.begin()+contador));
		//cout << mi_vector << endl;
		// Hago el recorrido
		bintree<Pregunta>::node nodo = arbol.root();
		bintree<Pregunta> otroArbol;
		bintree<Pregunta>::node aux, aux2;
		vector<bool>::iterator it1 = mi_vector.begin();
		vector<string>::iterator it2 = atributos.begin();
		while(!nodo.right().null() && !nodo.left().null()){
			//cout << (*nodo) << endl;
			if((*it1)==0){
				nodo = nodo.left();
			}
			else if((*it1)==1){
				nodo = nodo.right();
			}
			++it1;
			++it2;
		}
		//cout << *nodo << endl;
		aux = nodo; // lo Guardo
		nodo = nodo.parent(); // vuelvo al padre
		if(nodo.left()==aux){
			aux2 = nodo.right();
		}else if(nodo.right()==aux){
			aux2 = nodo.left();
		}
		//cout << *aux << " " << *aux2 << endl;
		// vuelvo al padre
		aux = nodo;
		nodo = nodo.parent();
		if(nodo.left()==aux){
			//arbol.prune_left(aux,otroArbol); // hago prune al otro, para guardar su valor
			if(aux.left()==aux2)
				arbol.prune_left(aux,otroArbol);
			else if(aux.right()==aux2)
				arbol.prune_right(aux,otroArbol);
			arbol.insert_left(nodo,otroArbol);
		}
		else if(nodo.right()==aux){
			//arbol.prune_right(aux,otroArbol); // hago prune al otro, para guardar su valor
			if(aux.left()==aux2)
				arbol.prune_left(aux,otroArbol);
			else if(aux.right()==aux2)
				arbol.prune_right(aux,otroArbol);
			arbol.insert_right(nodo,otroArbol);
		}
		(*nodo).reducir_personajes();
		// Los elimino de los vectores
		personajes.erase(personajes.begin()+contador);
		tablero.erase(tablero.begin()+contador);
	}
}

/////////////////////////////////////////////////
/// APARTADO SOBRE LA MEJORA: USANDO ENTOPIA ////
/////////////////////////////////////////////////

bintree<Pregunta> QuienEsQuien::crear_arbol_mejorado(){

	bintree<Pregunta>::node nodo;

	// Nodo Raiz
	vector<string> aux_atri = atributos;
	vector<string> aux_per = personajes;
	vector<vector<bool>> aux_tablero = tablero;
	int numel=personajes.size();
	int posicion = posicion_entropia(tablero,atributos);
	Pregunta p(atributos[posicion],numel);

	// Creo el arbol
	bintree<Pregunta> arbol(p);
	nodo = arbol.root();

	// Me aseguro de quitar el atributo usado de la lista
	aux_atri.erase(aux_atri.begin()+posicion);

	hijo_izquierda(tablero,posicion,aux_atri,nodo,aux_per);
	hijo_derecha(tablero,posicion,aux_atri,nodo,aux_per);

	return arbol;
}

int QuienEsQuien::posicion_entropia(vector<vector<bool>> matriz, vector<string> atri){

	vector<vector<bool>>::iterator it = matriz.begin();
	vector<bool>::iterator it2 = (*it).begin();
	vector<bool>::iterator it3 = it2;
	int total = 0;
	double p_unos, p_ceros, entropia;
	double mejor_entropia=0;
	int posicion;

	for(int i=0; i<atri.size(); i++){

		while(it!=matriz.end()){
			if((*it2)){ // porque es booleano
				total++;
			}
			it++;
			it2=(*it).begin()+i;
		}

		p_unos = double(total)/double(personajes.size());
		//cout << p_unos << endl;
		p_ceros = double(1) - p_unos;
		//cout << p_ceros << endl;
		double prueba_a = -p_unos*log(p_unos);
		//cout << prueba_a << endl;
		entropia = -p_unos*log2(p_unos) - p_ceros*log2(p_ceros);

		if(entropia>mejor_entropia){
			mejor_entropia;
			posicion = i;
		}
		// Reseteo
		total=0;
		it = matriz.begin();
		it2 = (*it).begin()+i+1; // uno mas porque es donde empezare ahora
	}

	//cout << "El atributo con mayor entropia es: " << atributos[posicion] << endl;
	return posicion;
}

void QuienEsQuien::hijo_izquierda(vector<vector<bool>> matriz, int posicion, vector<string> atri, bintree<Pregunta>::node n,vector<string> per){
	vector<vector<bool>> nueva = pelador(matriz,posicion,0,per); // para valores de izquierda
	vector<vector<bool>>::iterator it;
	bintree<Pregunta>::node mi_nodo = n;

	if(atri.size()>1 && nueva.size()>1 && !nueva.empty()){ // si tengo varios atributos, tomo decisiones
		vector<string> a = atri;
		int nueva_posicion = posicion_entropia(nueva,a);

		// Guardo dicho atributo como mi nuevo nodo
		Pregunta p(a[nueva_posicion],nueva.size());
		arbol.insert_left(mi_nodo,p);
		mi_nodo = n.left();
		a.erase(a.begin()+nueva_posicion);

		//metodo recursivo
		hijo_izquierda(nueva,nueva_posicion,a,mi_nodo,per);
		hijo_derecha(nueva,nueva_posicion,a,mi_nodo,per);
	}else if(atri.size()==1){
		// Como solo tengo un argumento, lo aniado en ambos sitios
		if(nueva.size()==1){
			Pregunta p(per[0],nueva.size());
			arbol.insert_left(mi_nodo,p);
		}else{
			Pregunta p(atri[0],nueva.size());
			arbol.insert_left(mi_nodo,p);
		// como mucho tendra tamanio 2, asi que puedo hacerlo manualmente
		// Como tengo dos, debo ver si el primero es 0 o 1
			vector<vector<bool>>::iterator ap = nueva.begin();
			vector<bool>::iterator ab = (*ap).begin();
			// si el primer personaje esta a 0
			if ((*ab)==0){
				mi_nodo = mi_nodo.left();
				Pregunta pa(per[0],1);
				Pregunta pb(per[1],1);
				arbol.insert_left(mi_nodo,pa);
				arbol.insert_right(mi_nodo,pb);
			// si el primer personaje esta a 1
			}else{
				mi_nodo = mi_nodo.left();
				Pregunta pa(per[1],1);
				Pregunta pb(per[0],1);
				arbol.insert_left(mi_nodo,pa);
				arbol.insert_right(mi_nodo,pb);
			}
		}
	}else if(nueva.size()==1 && atri.size()>1){
		Pregunta p(per[0],nueva.size());
		arbol.insert_left(mi_nodo,p);
	}


}

void QuienEsQuien::hijo_derecha(vector<vector<bool>> matriz, int posicion, vector<string> atri, bintree<Pregunta>::node n, vector<string> per){
	vector<vector<bool>> nueva = pelador(matriz,posicion,1,per); // para valores de derecha
	vector<vector<bool>>::iterator it;
	bintree<Pregunta>::node mi_nodo = n;

	if(atri.size()>1 && nueva.size()>1 && !nueva.empty()){ // si tengo varios atributos, tomo decisiones
		vector<string> a = atri;
		int nueva_posicion = posicion_entropia(nueva,a);

		// Guardo dicho atributo como mi nuevo nodo
		Pregunta p(a[nueva_posicion],nueva.size());
		arbol.insert_right(mi_nodo,p);
		mi_nodo = n.right();
		a.erase(a.begin()+nueva_posicion);

		//metodo recursivo
		hijo_izquierda(nueva,nueva_posicion,a,mi_nodo,per);
		hijo_derecha(nueva,nueva_posicion,a,mi_nodo,per);
	}else if(atri.size()==1){
		// Como solo tengo un argumento, lo aniado en ambos sitios
		Pregunta p(atri[0],nueva.size());
		arbol.insert_right(mi_nodo,p);
		// Como solo tengo un argumento, lo aniado en ambos sitios

		if(nueva.size()==1){
			Pregunta p(per[0],nueva.size());
			arbol.insert_right(mi_nodo,p);
		}else{
			Pregunta p(atri[0],nueva.size());
			arbol.insert_right(mi_nodo,p);

		// Como tengo dos, debo ver si el primero es 0 o 1
			vector<vector<bool>>::iterator ap = nueva.begin();
			vector<bool>::iterator ab = (*ap).begin();
			// Si el primer personaje esta a 0
			if ((*ab)==0){
				mi_nodo = mi_nodo.right();
				Pregunta pa(per[0],1);
				Pregunta pb(per[1],1);
				arbol.insert_left(mi_nodo,pa);
				arbol.insert_right(mi_nodo,pb);
			// Si el primer personaje esta a 1
			}else{
				mi_nodo = mi_nodo.right();
				Pregunta pa(per[1],1);
				Pregunta pb(per[0],1);
				arbol.insert_left(mi_nodo,pa);
				arbol.insert_right(mi_nodo,pb);
			}
		}
	}else if(nueva.size()==1 && atri.size()>1){
		Pregunta p(per[0],nueva.size());
		arbol.insert_right(mi_nodo,p);
	}

}

vector<vector<bool>> QuienEsQuien::pelador(vector<vector<bool>> matriz, int posicion, bool valor, vector<string> & per){

	vector<vector<bool>> nueva = matriz;
	vector<vector<bool>>::iterator it = nueva.begin();
	vector<string>::iterator its = per.begin();
	vector<bool>::iterator it2;

	while(it!=nueva.end()){
		it2 = (*it).begin() + posicion;
		if((*it2)!=valor){
			it = nueva.erase(it);
			its = per.erase(its);
		}else{
			(*it).erase(it2); // lo tengo que quitar
			it++;
			its++;
		}
	}
	return nueva;
}

/**
 * @brief Genera numero enteros positivos aleatorios en el rango [min,max).
**/

int generaEntero(int min, int max){
    int tam= max -min;
    return ((rand()%tam)+min);
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
	srand(0);

	this->limpiar();

	float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);

	int numero_de_atributos = ceil(log_2_numero_de_personajes);

	cout << "Petici�n para generar "<< numero_de_personajes<<" personajes ";
	cout << "con "<<numero_de_atributos<< " atributos"<<endl;
	cout << "Paso 1: generar "<< pow(2,numero_de_atributos) << " personajes"<<endl;

	//Fase 1: completar el tablero con todos los personajes posibles
	//Completo el tablero y los nombres de personajes a la vez
	for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
		vector<bool> atributos_personaje = convertir_a_vector_bool(indice_personaje,numero_de_atributos);
		string nombre_personaje = "Personaje_"+to_string(indice_personaje);

		this->personajes.push_back(nombre_personaje);
		this->tablero.push_back(atributos_personaje);
	}

	// Completo los nombres de los atributos.
	for(int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
		string nombre_atributo = "Atr_"+to_string(indice_atributo);
		this->atributos.push_back(nombre_atributo);
	}

	cout << "Paso 2: eliminar "<< (pow(2,numero_de_atributos)-numero_de_personajes) << " personajes"<<endl;
	//Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
	while(personajes.size()>numero_de_personajes){
		int personaje_a_eliminar = generaEntero(0,personajes.size());

		personajes.erase(personajes.begin()+personaje_a_eliminar);
		tablero.erase(tablero.begin()+personaje_a_eliminar);
	}
}
