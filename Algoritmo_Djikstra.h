#pragma once
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>

struct CaminosMinimos {
	std::vector<std::vector<int>> caminos; // lista de caminos (cada uno es lista de vértices)
	int distancia; // distancia mínima
};

class Grafo
{
private:
	int numNodos;
	std::vector<std::vector<int>> matrizAdyacencia;
	//Hola 
public:
	Grafo(int n) : numNodos(n), matrizAdyacencia(n, std::vector<int>(n, 0)) {}
	Grafo() : numNodos(0) {}
	void GenerarAleatorio(int n);

	/* Carga una matriz de adyacencia manualmente.Devuelve true si la matriz
	se cargó y el grafo resultante es acíclico, false si hay ciclos o la
	matriz no es válida. */
	bool CargarDesdeMatriz(const std::vector<std::vector<int>>& mat);

	// Vacía el grafo (sin nodos ni aristas)
	void Vaciar();

	/*Intenta agregar una arista u->v con peso; si al agregarla se mantiene
	acíclico la añade y devuelve true; si crearía un ciclo no la añade y
	devuelve false. */
	bool AgregarAristaSiAcyclic(int u, int v, int peso);

	// Encuentra TODOS los caminos mínimos desde s hasta t
	CaminosMinimos ObtenerCaminosMinimos(int s, int t);

	int ObtenerNumNodos() { return numNodos; }
	std::vector<std::vector<int>>& ObtenerMatriz() { return matrizAdyacencia; }
};