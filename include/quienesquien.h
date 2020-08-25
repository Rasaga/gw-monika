/*
Estructura de Datos 2017/2018 - Practica Final "Quien es Quien"
Nombre: Ramon Sanchez Garcia
Correo: chentaco@correo.ugr.es
*/

/**
 * @file quienesquien.h
 * @brief Fichero cabecera del QuienEsQuien.
 *
 * Almacena el arbol de preguntas para jugar al Quien es quien?.
 */

#ifndef _QUIENESQUIEN_H_
#define _QUIENESQUIEN_H_

#define DEBUG_QUIENESQUIEN 0

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <set>
#include <sstream>

#include "pregunta.h"
#include "bintree.h"

using namespace std;

/**
 * @brief T.D.A. QuienEsQuien
 * @author Ramon Sanchez Garcia
 *
 * Conjunto de personajes con sus atributos.
 * Lo primero que he hecho ha sido hacer el árbol de forma normal. Creo mi
 * arbol, como explico en la seccion "crear_arbol", usando los datos leidos.
 *
 * Se ha implementado tambien una mejora que crea el arbol usando entropia. Para
 * ver como lo he hecho, consulta la seccion "crear_arbol_mejorado".
 *
 * Finalmente he implementado tambien los metodos obligatorios y opcionales.
 * Toda la informacion de como los he hecho lo explico en sus correspondientes metodos.
 */
class QuienEsQuien{
private:
	/**
	  * @brief Almacena el nombre de los personajes. Su indice en
	  *        este vector corresponde con el indice de su
	  *        descripcion en el tablero.
	  */
	vector<string> personajes;

	/**
	  * @brief Almacena los atributos.  Su indice en este vector
	  *        corresponde con el indice correspondiente de cada
	  *        vector del tablero.
	  */
	vector<string> atributos;

	/**
	  * @brief Matriz que almacena para cada personaje su valor para
	  *        cada atributo.
	  */
	vector<vector<bool>> tablero;

	/**
	  * @brief Arbol de preguntas para adivinar personajes.
	  */
	bintree<Pregunta> arbol;

	/**
	  * @brief Nodo del arbol de preguntas correspondiente a la jugada
	  *        actual.
	  */
	bintree<Pregunta>::node jugada_actual;
public:
	/**
	  * @brief Constructor basico de la clase
	  */
	QuienEsQuien();
	/**
	  * @brief Constructor de copia.
	  * @param quienEsQuien QuienEsQuien a copiar.
	  */
	QuienEsQuien(const QuienEsQuien &quienEsQuien);
	/**
	* @brief Destructor del pregunta.
	*/
	~QuienEsQuien();
	/**
	  * @brief Vacia todos los datos del QuienEsQuien receptor.
	  */
	void limpiar();
	/**
	* @brief Sobrecarga del operador de asignacion.
	* @param quienEsQuien objeto a copiar.
	* @return Referencia al objeto copiado.
	*/
	QuienEsQuien& operator = (const QuienEsQuien &quienEsQuien);

	/**
	* @brief Operador de extraccion de flujo.
	* @param is Stream de entrada
	* @param quienEsQuien Quien es quien a leer.
	* @return Referencia al stream de entrada.
	*
	* Lee de @e is un quien es quien y lo almacena en @e
	* quienEsQuien. El formato aceptado para la lectura es un TSV
	* (tab-separated values) en el que las columnas tienen cabecera y
	* son las preguntas. La ultima columna corresponde al nombre del
	* personaje. Tras la cabecera se especifica en cada lnea un
	* personaje, teniendo el valor 1 o 0 si tiene/no tiene el
	* atributo de la columna. En la ultima columna se da el
	* nombre del personaje.
	*/
	friend istream& operator >> (istream& is, QuienEsQuien &quienEsQuien);

	/**
	* @brief Operador de insercion de flujo.
	* @param os Stream de salida
	* @param quienEsQuien Quien es quien a escribir.
	* @return Referencia al stream de salida.
	*
	* Escribe en @e is un quien es quien.
	* El formato usado para la escritura es un TSV
	* (tab-separated values) en el que las columnas tienen cabecera y
	* son las preguntas. La ultima columna corresponde al nombre del
	* personaje. Tras la cabecera se especifica en cada linea un
	* personaje, teniendo el valor 1 o 0 si tiene/no tiene el
	* atributo de la columna. En la ultima columna se da el
	* nombre del personaje.
	*/
	friend ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien);

	/**
	  * @brief Escribe en la salida estandard las estructuras atributos, personajes y tablero.
	  */
	void mostrar_estructuras_leidas();

	/**
	  * @brief Este metodo construye el arbol de preguntas para todos los personajes del tablero.
	  */
	bintree<Pregunta> crear_arbol();

	/**
	  * @brief Sustituye el arbol actual por el arbol pasado por parmetro.
	  *
	  * @param arbol_nuevo Arbol de preguntas que sustituye al actual.
	  *
		* Para crear el arbol he hecho un proceso recursivo. En primer lugar creo
		* el arbol cuya raiz es el primer atributo del vector de atributos.
		* Ahora empiezo a moverme. Si es 0, me muevo a hijo izquierda; si es 1, me
		* muevo al hijo derecha. Ahi inserto el siguiente atributo. Asi hasta llegar
		* al final, donde insertare el personaje correspondiente, es decir, el primero
		* de la lista de personajes. Repito el proceso con el resto de filas, de nuevo,
		* desde el root.
		* Pero hay que tener algo en cuenta: si el nodo ya existe, NO SE AÑADE NADA.
		* Este proceso va bien, pero hay un pequeño problema, ¿como calculo el numero
		* de hijos antes de insertar nada?
		*
		* La primera idea que tuve fue la de insertar un numero de hijos de tamaño 2,
		* salvo que no sea el ultimo, y luego hacer otro recorrido e ir editandolo.
		* Pero he tenido una idea mejor, donde solo he de recorrer el arbol la mitad
		* de veces.
		* Cuando leo un vector para insertar un nodo, dicho vector booleano lo
		* compruebo compruebo con los que le quedan "por debajo", es decir, los
		* que hay desde la posicion donde esta el hasta el final.
		*
		* Dicho de otra forma, voy a crear un iterador a la posicion de ese vector,
		* y lo voy a comprobar a los que hay desde ese hasta el final.
		* Pero no voy a comprobar el vector entero, sino que cada vez lo comparo hasta
		* la posicion de dicho atributo. Es decir:
		* Si tengo un vector [0 1 1] y lo que voy a añadir es el primer atributo, el que
		* vale a "0" en este vector, he de comprobar cuantos tienen tambien como primera
		* decision "0", para el siguiente atributo tengo que buscar cuantos empiezan por
		* "01", para el siguiente "011"...
		* La cantidad de vectores que empiecen por eso que coincidan, seran el numero de hijos.
		*
		* Esto puede provocar un error, ya que puedo añadir un nodo que ya exista con menor
		* si hago esto... Pero dado a como busco el arbol, desde el primero al ultimo, y dado
		* que no puedo añadir nodos a posiciones donde ya hay uno, esto funciona correctamente.
		* Me explico: supongo que recorro el primer vector y ya he hecho un primer recorrido.
		* Con eso tengo un nodo X que tiene 3 hijos. Ahora haciendo el segundo recorrido para el
		* segundo vector, me sale ese mismo nodo con 2 hijos. No puedo añadirlo porque ya existia uno antes.
		* Ademas, dada esta expliracion, siempre se añaden los nodos con el numero de hijos correcto.
	  */
	void usar_arbol(bintree<Pregunta> arbol_nuevo);

	/**
	 * @brief Inicializa el juego.
	 * @post  Si la partida anterior no haba terminado se
	 *        pierde el progreso.
	 * He creado un "robot" llamado Monika, el cual te hara preguntas. Dependiendo
	 * de la respuesta, preguntara por un nuevo atributo o encontrara el personaje.
	 */
	void iniciar_juego();

	/**
	  * @brief Dado un estado del tablero devuelve los nombres de
	  *        los personajes que aun no han sido tumbados en el
	  *        tablero.
	  *
	  * @param jugada_actual Nodo del estado del tablero.
	  *
	  * @return Conjunto de personajes que no han sido tumbados en el tablero.
	  *
	  * @pre El arbol del QuienEsQuien receptor debe haber sido construido previamente.
	  * @pre El nodo indicado debe ser un nodo del arbol del QuienEsQuien receptor.
		*
		* Devuelve un set con todos los personajes que aun son hijos de ese nodo. Es
		* decir, cuando llego a un nodo, busco todos las hojas que hay a partir de ese nodo
		* y las voy insertando en un set. Realizo este proceso recursivamente para buscar
		* y almaceno el resultado en un set. Para hacer esto he declarado el union_jugadas.
		*
		* Finalmente obtendre un set con todos los jugadores/nodos hojas a partir de ese nodo
	  */

	set<string> informacion_jugada(bintree<Pregunta>::node jugada_actual);

	/**
		* @brief Metodo auxiliar, utilizado para unir dos set
		* @param primera El primer set
		* @param segunda El segundo set
		* @return union La union de las dos jugadas
		*
		* Implemento este metodo dado a que informacion_jugada obtendra una jugada,
		* pero quiero un conjunto de jugadas. Por eso voy uniendo cada nueva jugada
		* al resto de jugadas.
	*/

	set<string> union_jugadas(set<string> primera, set<string> segunda);

	/**
	  * @brief Escribe el arbol generado en la salida estandard.
	  */
	void escribir_arbol_completo() const;

	/**
	   * @brief Metodo que modifica el arbol de preguntas para que haya
	   *        preguntas redundantes.
	   * @post El arbol de preguntas se modifica.
		 *
		 * Esta funcion llama a eliminar_nodos_redundantes(arbol.root()).
	*/

	void eliminar_nodos_redundantes();

	/**
		* @brief Metodo que modifica el arbol de preguntas para que haya
		*        preguntas redundantes.
		* @param n nodo sobre el que actuara.
		* @post El arbol de preguntas se modifica.
		*
		* A diferencia de su version sin parametro, esta recibe un nodo, y
		* comprueba si ese nodo es nulo. En caso de que no lo sea, ajustara
		* los nodos nulos que tenga. En caso de que lo tenga, vuelvo a ejecutarse
		* el mismo proceso desde el root() del arbol (el motivo es que cuando
	 	* haga el ajuste, puede que aparezcan nuevos nodos nulos, he de volver a
	 	* comprobar desde arriba).
		* Si no puedo ajustar, compruebo a partir de los nodos izquierda y derecha.
		*
		* NOTA: Dado que he estado solo, tuve que pedir ayuda a otros compañeros sobre
		* este metodo.
	*/

	void eliminar_nodos_redundantes(bintree<Pregunta>::node n);

	/**
		* @brief Metodo auxiliar para comprobar si un nodo tiene hijos nulos.
		* @param n nodo que quiero comprobar
		* @return valor booleano que sera falso si no tiene, true si tiene alguno
		*
		* Este metodo sera usado para comprobar si un hijo tiene algun hijo nulos
		* para eliminar hijos redundantes.
		*/

	bool tengo_hijos_nulos(bintree<Pregunta>::node n);

	/**
		*	@brief Metodo para ajustar los hijos nulos
		* @param n nodo sobre el que trabajare
		* @return valor booleano que sera true si he modificado el arbol, false
		* si no he realizado nada.
		*
		* Metodo con mas "chicha" sobre como eliminar nodos redudantes. Y que mas
		* dolor de cabeza me ha traido. Mi implementacion consiste en lo siguiente:
		* Voy a comprobar si mi nodo "n" tiene algun hijo nulo. Si lo tiene, y
		* ademas ese nodo no es el root (imposible que lo sea ademas, pero puede
		* pasar), compruebo si ese nodo es hijo izquierdo o derecho de su padre.
		* Dependiendo de la opcion, ahora compruebo cual de sus hijos es nulo, cambio
		* dicho hijo por su padre usando los metodos prune e insert, y cambio el valor
		* booleano de control a true (creado inicialmente a false).
		*
		* En caso que se de que el nodo root tenga un hijo nulo, el prune lo hago de
		* otra forma, si subirme a la posicion padre del root(), ya que no existe.
		*
		* Este metodo devuelve el valor booleano que sera utilizado por la funcion
		* eliminar_nodos_redundantes(node).
		*/

	bool ajuste_nodos_nulos(bintree<Pregunta>::node n);

	/**
		* @brief Este metodo permite calcular la media de la profundidad de las hojas del arbol. Este valor representa el numero (promedio) de preguntas necesarias para adivinar cada personaje. A menor profundidad promedio, mejor solucion. Esta metrica es un indicador para evaluar la calidad de vuestra solucion.
		*
		* @return Profundidad media del arbol de preguntas.
		*
		* @pre El arbol de preguntas debe haber sido construido previamente.
		*
		* Como se vio en teoria, la profundidad promedia se calcula como la suma de
		* profundidades totales (para ello uso el metodo total_profundidad()) entre
		* el numero total de nodos (aunque ya tiene uno bintree, como esta privado
		* y no quiero tocar el fichero para nada, voy a crear y usar el metodo
		* contar())
		*/
	float profundidad_promedio_hojas();

	/**
		* @brief Metodo para contar el numero de nodos a partir de un nodo.
		* @param nodo el nodo desde el que quiero empezar a contar
		* @return total numero de nodos
		*
		* Aunque ya tiene uno bintree, como es privado y no quiero tocar ese
		* fichero, voy a crearme uno propio.
		*
		* Se utilizara para calcular la profundiad promedio.
	*/

	int contar(bintree<Pregunta>::node nodo);

	/**
		* @brief Metodo para obtener el total de profundidades de cada nodo
		* @param n nodo al que quiero calcular la profundidades
		* @int acum total de "pronfundidades", que ira aumentando
		*
		* Este metodo se encarga de calcular el total de profundidad del arbol
		* que se calcula como la suma de la profundidad de todos los nodos.
		*
		* Se utilizara para calcular la profundiad promedio.
	*/

	int total_profundidad(bintree<Pregunta>::node n, int acum);

	/**
		* @brief Metodo que devuelve un par de valores con la informacion
		* de la jugada actual.
		* @param n nodo en cuestion que hemos consultado
		* @param valor decision escogida
		*	@return pair con los string del atributo y su respuesta (si/no)
		*
		*	Este metodo se encarga de recibir un nodo y lo que hemos respondido,
		* y devolver un pair de la forma pair<string, string> con el nombre del
		* del atributo y su contestacion. Todos estos pair, como dije, se almacenan
		* en el metodo jugar, dentro de un vector<pair<string,string>>, el cual
		* se va recorriendo cada vez que he jugado.
	*/

	pair<string,string> info_jugada(bintree<Pregunta>::node n, bool valor);

	/**
		* @brief Metodo para insertar un jugador a mi arbol
		* @param nombre el jugador que quiero insertar
		* @param nuevo el vector de decisiones, que corresponde a los parametros
		*
		* Este metodo se encargara de buscar el nodo, mientras no tenga hijos nulos,
		* usando el vector booleano. Cuando llega a esa posicion, que SIEMPRE SERA una
		* hoja, cojo esa hoja, la guardo en una funcion auxiliar. Vuelvo al nodo padre
		* e inserto ambos hijos. Dado a la implementacion, el nuevo jugador no
		* tendra los mismos atributos que el que ya existia. Por ello inserto ese nuevo
		* jugador en la parte que corresponda, y el otro que ya teniamos, en la otra.
		* Ademas el atributo de ese nodo padre sera ahora el que corresponda de mi
		* vector de atributos, el cual me he ido desplazando a la vez que me desplazaba
		* por el arbol.
		* NOTA IMPORTANTE: Este metodo no funciona correctamente con la VERSION MEJORADA
		* debido a como estan organizados los atributos en el otro
	*/

	void insertar_jugador(string nombre,vector<bool> nuevo);

	/**
		* @brief Metodo auxiliar usando por insertar_jugador que comprueba si un
		* vector booleano existe o no en el tablero.
		* @param prueba el vector que deseo buscar
		* @return esta a true o false si existe
		*
		* Lo que hace es buscar este vector en el tablero que teniamos. Dada la
		* estructura del tablero, puedo comprobar cada fila con este vector usando
		* un iterador que recorre todas las posiciones con el vector.
		* Si lo encuentra, devuelve true, en caso contrario, false.
	*/

	bool esta_vector(vector<bool> prueba);

	/**
		* @brief Metodo que elimina un jugador pasado por parametro de un arbol
		* @param nombre El nombre del jugador que deseo eliminar
		*
		* Lo que hace este metodo es buscar el vector de atributos en el tablero
		* que corresponde a dicho personaje. Si no lo encuentra, mostrara que no existe.
		* En caso de que lo encuentre, voy a hacer un recorrido iterativo desde la Raiz
		* del arbol con ese vector booleano. Cuando llegue al hijo, insertara como padre
		* la otra rama. Para eso, utiliza la funcion prune, e inserta en el nodo padre
		* de este la nueva rama.
		*
		* Adicionalmente reduce el numero de jugadores del nodo padre en 1.
		* NOTA IMPORTANTE: Este metodo no funciona correctamente con la VERSION MEJORADA
	*/

	void eliminar_jugador(string nombre);

	/**
		* @brief Funcion que crea un nuevo arbol mejorado. Para ello, utiliza la entropia del tablero para crear
		* un nuevo arbol. Cada pregunta que se crea nuevamente, dependera del valor booleano y de la entropia de los
		* valores que queden, que no hayan sido usados. Para ello, al contrario de como hice el arbol normal, use
		* estructuras auxiliares. Para hacer uso de esto, ademas implemente varios metodos adicionales:
		* posicion_entropia(matriz,atri), hijo_izquierda(matriz,posicion,atri,n,per),
		* hijo_izquierda(matriz,posicion,atri,n,per);
		* NOTA: Podria haber hecho una funcion para llamar a estos dos ultimos metodos. Por falta de tiempo, junto a otros, lo
		* he dejado asi.
		* @return El nuevo arbol mejorado
	*/

	bintree<Pregunta> crear_arbol_mejorado();

	/**
		* @brief Metodo que devuelve la posicion del atributo con mejor entropia.
		* @param matriz tablero donde se aplicara la entropia
		* @param atri vector de los atributos correspondientes
		* @return Devuelve la posicion del atributo con mejor entropia.
		*
		* Lo he diseñado asi dado que cuando obtenga la posicion del atributo con
		* mejor entropia, creare el nuevo objeto Pregunta con la posicion de este atributo.
		* Le paso ademas un array de atributos dado a que, como dije, estoy usando
		* estructuras auxiliares, por lo que el vector de atributos sera diferente cada vez,
		* por lo que hay que actuar sobre uno nuevo cada vez.

	*/

	int posicion_entropia(vector<vector<bool>> matriz, vector<string> atri);

	/**
		* @brief Metodo que inserta un hijo en la izquierda.
		* @param posicion la posicion del atributo con mejor entropia
		* @param matriz la matriz que tengo actualmente
		* @param atri el vector de atributos que tengo actualmente
		* @param n el nodo de pregunta en el que estamos
		* @param per vector de personajes que tengo actualmente
		*
		* Lo que hace esta funcion en primer lugar es pelar dicha matriz, usando
		* para ello la posicion el valor "0", junto a "matriz" y atributos. Usa 0
		* porque esta a la izquierda. Guarda la nueva matriz en una variable llamada
		* "nueva". Despues, usando esa nueva matriz, si hay varios personajes y
		* atributos (usa el numero de filas de la nueva matriz y el tamaño de "atri"),
		* busca el de mayor entropia, y lo inserta. Si solo hay un atributo, pero uno
		* o dos (no puede haber mas debido a la implementacion), añadira estos personajes
		* manualmente. En caso que haya varios atributos pero un personaje solo, añade
		* ese personaje directamente.

		* Para ahorrarme problemas, copio el vector de atributos a otro, llamado "a".
		* De dicho vector, le quito los atributos insertados. Si siguen quedando,
		* ejecuto por derecha e izquierda.

		* En caso de que aun haya personajes que añadir, ejecuto recursivamente este metodo
		* y el de la parte derecha, con los nuevos objetos creados.
	*/

	void hijo_izquierda(vector<vector<bool>> matriz, int posicion, vector<string> atri, bintree<Pregunta>::node n, vector<string> per);
	/**
		* @brief Metodo que inserta un hijo en la derecha.
		* @param posicion la posicion del atributo con mejor entropia
		* @param matriz la matriz que tengo actualmente
		* @param atri el vector de atributos que tengo actualmente
		* @param n el nodo de pregunta en el que estamos
		* @param per vector de personajes que tengo actualmente

		* Metodo analogo al de izquierda. Realiza lo mismo, pero en la parte derecha
		* NOTA: Podria haber hecho un metodo que hiciese ambas cosas a la vez. Pero no
		* se me ha ocurrido como, ni tengo tiempo. Estoy haciendo todo esto solo, y
		* no dispongo de mas tiempo. Siento mucho que la implementacion sea tan "guarra",
		* pero no se que hacer ya con tan poco tiempo.
		*/

	void hijo_derecha(vector<vector<bool>> matriz, int posicion, vector<string> atri, bintree<Pregunta>::node n, vector<string> per);

	/**
		* @brief Se encarga de crear una nueva matriz a partir de otra, la posicion del valor, el valor en cuestion (0 o 1)
		* y un vector de persojes.
		* @param matriz, es la matriz original que queremos editar
		* @param posicion, la posicion del valor en la matriz
		* @param valor, el valor que queremos buscar. Eliminamos aquellas filas a partir de la posicion que no sea
		* de este valor.
		* @param per, es un vector de personajes del que se eliminara los personajes a la vez que se eliminan las
		* filas del tablero. Se elimina el personaje correspondiente.
		* @return La nueva matriz "pelada".
		*
		* Dicha funcion buscara la posicion en la nueva matriz, y se quedara con las filas cuyo valor,
		* en la posicion que le hemos pasado por parametro, sea el indicado por parameto. Adicionalmente borra los personajes
		* a los que corresponden esas filas de valores. Tambien borra la columna. Devuelve la nueva matriz.
		*/

	vector<vector<bool>> pelador(vector<vector<bool>> matriz, int posicion, bool valor, vector<string> & per);

	/**
	  * @brief Rellena los datos del QuienEsQuien con un tablero calculado aleatoriamente.
	  *
	  * @param numero_de_personajes Numero de personajes que tiene el tablero a crear.
	  */
	void tablero_aleatorio(int numero_de_personajes);
};

#endif
