/* B-Tree sample code 27.04.2018
*  Code is based from the https://www.geeksforgeeks.org/b-tree-set-1-insert-2/
*  Halil DURMU� ITU BLG22
*/
// C++ program for B-Tree insertion
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// A BTree node   --- // Un nodo del BTree
class BTreeNode
{
	int* keys;  // An array of keys   --- Una matriz de claves
	int t;      // Minimum degree (defines the range for number of keys)  --- Grado mínimo (define el rango para el número de teclas)
	BTreeNode** C; // An array of child pointers   ----  Una matriz de punteros hijos
	int n;     // Current number of keys   ---- Número actual de llaves
	bool leaf; // Is true when node is leaf. Otherwise false   ---- Es verdadero cuando el nodo es una hoja. En caso contrario, es falso
public:
	BTreeNode(int _t, bool _leaf);   // Constructor

	// A utility function to insert a new key in the subtree rooted with  ---  Una función de utilidad para insertar una nueva clave en el subárbol enraizado con
	// this node. The assumption is, the node must be non-full when this  ---   este nodo. La suposición es que el nodo debe estar no lleno cuando esta
	// function is called  --- función es llamada
	void insertNonFull(int k);

	// Una función de utilidad para dividir el hijo y de este nodo. i es el índice de y en
	// matriz de hijos C[].  El hijo y debe estar lleno cuando se llame a esta función
	void splitChild(int i, BTreeNode* y);

	// A function to traverse all nodes in a subtree rooted with this node
	// Una función para recorrer todos los nodos de un subárbol enraizado con este nodo
	void traverse();

	// A function to search a key in subtree rooted with this node. ---  Una función para buscar una clave en el subárbol enraizado con este nodo.
	BTreeNode* search(int k);   // returns NULL if k is not present.  --- devuelve NULL si k no está presente.

	// Make BTree friend of this so that we can access private members of this  --- Haz a BTree amigo de esto para que podamos acceder a los miembros privados de este
	// class in BTree functions  --- en las funciones de BTree
	friend class BTree;
};

// A BTree --- Un árbol BT
class BTree
{
	BTreeNode* root; // Puntero al nodo raíz
	int t;  // Grado mínimo
public:
	// Constructor (Initializes tree as empty)   ---- Constructor (Inicializa el árbol como vacío)
	BTree(int _t)
	{
		root = NULL;  t = _t;
	}

	// function to traverse the tree   ------ para recorrer el árbol
	void traverse()
	{
		if (root != NULL) root->traverse();
	}

	// para buscar una clave en este árbol
	BTreeNode* search(int k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// La función principal que inserta una nueva clave en este árbol B
	void insert(int k);
};

// Constructor for BTreeNode class   ----  Constructor de la clase BTreeNode
BTreeNode::BTreeNode(int t1, bool leaf1)
{
	// Copiar el grado mínimo dado y la propiedad de la hoja
	t = t1;
	leaf = leaf1;

	// Asignar memoria para el máximo número de claves posibles
	// y punteros hijos
	keys = new int[2 * t - 1];
	C = new BTreeNode * [2 * t];

	// Inicializar el número de llaves como 0
	n = 0;
}

// Función para recorrer todos los nodos de un subárbol enraizado con este nodo
void BTreeNode::traverse()
{
	// Hay n llaves y n+1 hijos, recorre las n llaves
	// y los primeros n hijos
	int i;
	for (i = 0; i < n; i++)
	{
		// Si esto no es una hoja, entonces antes de imprimir la llave[i],
		// recorrer el subárbol enraizado con el hijo C[i].
		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i];
	}

	// Imprime el subárbol enraizado con el último hijo
	if (leaf == false)
		C[i]->traverse();
}

// Función para buscar la clave k en el subárbol enraizado con este nodo
BTreeNode* BTreeNode::search(int k)
{
	// Encuentra la primera clave mayor o igual a k
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	// Si la clave encontrada es igual a k, devuelve este nodo
	if (keys[i] == k)
		return this;

	// Si la clave no se encuentra aquí y este es un nodo hoja
	if (leaf == true)
		return NULL;

	// Ir al hijo apropiado
	return C[i]->search(k);
}

// The main function that inserts a new key in this B-Tree  ---  // La función principal que inserta una nueva clave en este B-Tree
void BTree::insert(int k)
{
	// If tree is empty   ------  Si el árbol está vacío
	if (root == NULL)
	{
		// Allocate memory for root    ----- Asignar memoria para la raíz
		root = new BTreeNode(t, true);
		root->keys[0] = k;  // Insert key    -----   Insertar llave
		root->n = 1;  // Update number of keys in root   -----  Actualizar el número de llaves en la raíz
	}
	else // If tree is not empty      -------   Si el árbol no está vacío
	{
		// If root is full, then tree grows in height    --------   Si la raíz está llena, el árbol crece en altura
		if (root->n == 2 * t - 1)
		{
			// Allocate memory for new root            ------   Asignar memoria para la nueva raíz
			BTreeNode* s = new BTreeNode(t, false);

			// Make old root as child of new root      ------    Hacer que la antigua raíz sea hija de la nueva
			s->C[0] = root;

			// Split the old root and move 1 key to the new root    ----- Dividir la antigua raíz y mover 1 tecla a la nueva raíz
			s->splitChild(0, root);

			// New root has two children now.  Decide which of the  ----- La nueva raíz tiene ahora dos hijos.  Decide cuál de los
			// two children is going to have new key            --------   dos hijos va a tener nueva llave
			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// Change root  --- Cambiar la raíz
			root = s;
		}
		else  // Si la raíz no está llena, llame a insertNonFull para la raíz
			root->insertNonFull(k);
	}
}

// A utility function to insert a new key in this node     ------- Una función de utilidad para insertar una nueva clave en este nodo
// The assumption is, the node must be non-full when this  -------  La suposición es que el nodo debe estar no lleno cuando este
// function is called                                      -------  se llama a la función
void BTreeNode::insertNonFull(int k)
{
	// Initialize index as index of rightmost element    ------ Inicializar el índice como índice del elemento más a la derecha
	int i = n - 1;

	// If this is a leaf node    ------- Si se trata de un nodo hoja
	if (leaf == true)
	{
		// The following loop does two things               -------- El siguiente bucle hace dos cosas
		// a) Finds the location of new key to be inserted  -------- Busca la ubicación de la nueva llave que se va a insertar
		// b) Moves all greater keys to one place ahead     -------- Mueve todas las llaves mayores a un lugar adelante
		while (i >= 0 && keys[i] > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		// Insert the new key at found location     --------- Introduzca la nueva llave en el lugar encontrado
		keys[i + 1] = k;
		n = n + 1;
	}
	else // If this node is not leaf     ------- Si este nodo no es hoja
	{
		// Find the child which is going to have the new key   ---------- Encuentre el hijo que va a tener la nueva llave
		while (i >= 0 && keys[i] > k)
			i--;

		// See if the found child is full       ---------- Ver si el hijo encontrado está lleno
		if (C[i + 1]->n == 2 * t - 1)
		{
			// If the child is full, then split it     ---------- Si el hijo está lleno, entonces divídelo
			splitChild(i + 1, C[i + 1]);

			// After split, the middle key of C[i] goes up and   -------- Después de la división, la clave media de C[i] sube y
			// C[i] is splitted into two.  See which of the two    ------ C[i] se divide en dos.  Vea cuál de los dos
			// is going to have the new key   ------- va a tener la nueva clave
			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

// A utility function to split the child y of this node   -------- Una función de utilidad para dividir el hijo y de este nodo
// Note that y must be full when this function is called   ------- Tenga en cuenta que y debe estar lleno cuando se llame a esta función
void BTreeNode::splitChild(int i, BTreeNode* y)
{
	// Create a new node which is going to store (t-1) keys  -------- Crear un nuevo nodo que va a almacenar (t-1) claves
	// de y
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// Copy the last (t-1) keys of y to z -------- Copiar las últimas (t-1) claves de y en z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// Copy the last t children of y to z  ----- Copiar los últimos t hijos de y en z
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// Reducir el número de llaves en y
	y->n = t - 1;

	// Ya que este nodo va a tener un nuevo hijo,
	// creamos el espacio del nuevo hijo
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// Vincular el nuevo hijo a este nodo
	C[i + 1] = z;

	// Una clave de y se moverá a este nodo. Encuentra la ubicación de la
	// nueva tecla y mueve todas las teclas mayores un espacio adelante
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// Copiar la clave media de y a este nodo
	keys[i] = y->keys[t - 1];

	// Incrementa el número de claves en este nodo
	n = n + 1;
}

// Programa controlador para probar las funciones anteriores
int main()
{
	BTree t(3); // Un árbol B con un grado mínimo de 3

	int num;
	string nombreArchivo = "C:/Users/Antony/Downloads/test-100mb.txt";
	ifstream archivo(nombreArchivo.c_str());
	string linea;
	// Obtener línea de archivo, y almacenar contenido en "linea"
	while (getline(archivo, linea)) 
	{
		num = stoi(linea);

		t.insert(num);
	}
	/*
	t.insert(10);
	t.insert(20);
	t.insert(5);
	t.insert(6);
	t.insert(12);
	t.insert(30);
	t.insert(7);
	t.insert(17);*/

	cout << "Traversal of the constucted tree is ";
	t.traverse();

	int k = 6;
	(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

	k = 15;
	(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

	getchar();

	return 0;
}
/*
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main() 
{
    int cont = 1;
    string nombreArchivo = "C:/Users/Antony/Downloads/Nuevo documento de texto.txt";
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    // Obtener línea de archivo, y almacenar contenido en "linea"
    while (getline(archivo, linea)) {
        // Lo vamos imprimiendo
        //cout << cont++ <<". ";
        //cout << linea << endl;
    }

    return 0;
}*/
