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

	std::vector<std::pair<double, double>> posiciones;
	double aspecto = 1.0;

	//Hola 
public:
	Grafo(int n) : numNodos(n), matrizAdyacencia(n, std::vector<int>(n, 0)) {}
	Grafo() : numNodos(0) {}
	void GenerarAleatorio(int n, double aspecto = 1.0, double radio = -1.0);

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

	bool TienePosiciones() const { return !posiciones.empty(); }
	std::pair<double, double> ObtenerPos(int i) const { return posiciones[i]; }
	double ObtenerAspecto() const { return aspecto; }

	// Uso de la teoria de grafos conexos
	static bool esConexoNoDirigido(const std::vector<std::vector<int>>& m) {
		int n = (int)m.size();
		if (n <= 1) return true;
		std::vector<bool> visto(n, false);
		std::vector<int> pila;
		pila.push_back(0);
		visto[0] = true;
		int contados = 1;
		while (!pila.empty()) {
			int u = pila.back();
			pila.pop_back();
			for (int v = 0; v < n; v++) {
				if (!visto[v] && (m[u][v] != 0 || m[v][u] != 0)) {
					visto[v] = true;
					contados++;
					pila.push_back(v);
				}
			}
		}
		return contados == n;
	}



};