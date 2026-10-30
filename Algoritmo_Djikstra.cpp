#include "Algoritmo_Djikstra.h"
#include <functional>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <string> 
#define PI 3.14159265358979

void Grafo::GenerarAleatorio(int n, double asp , double radio) {
	const int PESO_MIN = 5;
	const int PESO_MAX = 50;

	numNodos = n;
	matrizAdyacencia.assign(n, std::vector<int>(n, 0));
	GenerarPosiciones(n, asp);
	if (n <= 0) return;
	if (asp <= 0) asp = 1.0;
	asp = aspecto;
	if (radio <= 0) {
		radio = (n >= 2) ? 1.5 * sqrt(asp * std::log((double)n) / (PI * n)) : 1.0;
	}

	while (true) {
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				double dx = posiciones[j].first - posiciones[i].first;
				double dy = posiciones[j].second - posiciones[i].second;
				double d = std::sqrt(dx * dx + dy * dy);
				matrizAdyacencia[i][j] = (d <= radio) ? PESO_MIN + rand() % PESO_MAX : 0;
			}
		}
		if (esConexoNoDirigido(matrizAdyacencia)) break;
		radio *= 1.1;
	}
}

bool Grafo::CargarDesdeMatriz(const std::vector<std::vector<int>>& mat) {
	int n = (int)mat.size();
	if (n == 0) return false;
	for (const auto& row : mat) if ((int)row.size() != n) return false;

	// Copiamos la matriz temporalmente
	matrizAdyacencia = mat;
	numNodos = n;

	// Detectar ciclos mediante DFS (colores: 0=blanco,1=gris,2=negro)
	std::vector<int> color(n, 0);
	std::function<bool(int)> dfs = [&](int u) -> bool {
		color[u] = 1;
		for (int v = 0; v < n; ++v) {
			if (matrizAdyacencia[u][v] != 0) {
				if (color[v] == 1) return true; // ciclo encontrado
				if (color[v] == 0 && dfs(v)) return true;
			}
		}
		color[u] = 2;
		return false;
		};


	for (int i = 0; i < n; ++i) {
		if (color[i] == 0) {
			if (dfs(i)) {
				matrizAdyacencia.assign(0, std::vector<int>());
				numNodos = 0;
				posiciones.clear();      
				return false;
			}
		}
	}

	GenerarPosiciones(n);               
	return true;
}

bool Grafo::AgregarAristaSiAcyclic(int u, int v, int peso) {
	if (u < 0 || v < 0 || u >= numNodos || v >= numNodos) return false;
	// copia temporal
	std::vector<std::vector<int>> temp = matrizAdyacencia;
	temp[u][v] = peso;

	int n = numNodos;
	std::vector<int> color(n, 0);
	std::function<bool(int)> dfs = [&](int node) -> bool {
		color[node] = 1;
		for (int w = 0; w < n; ++w) {
			if (temp[node][w] != 0) {
				if (color[w] == 1) return true;
				if (color[w] == 0 && dfs(w)) return true;
			}
		}
		color[node] = 2;
		return false;
		};

	for (int i = 0; i < n; ++i) {
		if (color[i] == 0) {
			if (dfs(i)) return false; // ciclo detectado
		}
	}

	// Si no hubo ciclo, aplicamos
	matrizAdyacencia = std::move(temp);
	return true;
}

void Grafo::Vaciar() {
	numNodos = 0;
	matrizAdyacencia.clear();
	posiciones.clear();
}

CaminosMinimos Grafo::ObtenerCaminosMinimos(int s, int t) {
	CaminosMinimos resultado;
	resultado.distancia = std::numeric_limits<int>::max();
	resultado.caminos.clear();

	if (s < 0 || s >= numNodos || t < 0 || t >= numNodos || s == t) {
		return resultado; // caso inválido o trivial
	}

	// Dijkstra para encontrar distancias mínimas desde s
	const int INF = std::numeric_limits<int>::max() / 2;
	std::vector<int> dist(numNodos, INF);
	std::vector<std::vector<int>> prev(numNodos); // para cada nodo, lista de predecesores en caminos mínimos
	std::vector<bool> vis(numNodos, false);

	dist[s] = 0;

	for (int iter = 0; iter < numNodos; ++iter) {
		// seleccionar nodo no visitado con menor distancia
		int u = -1;
		int best = INF;
		for (int i = 0; i < numNodos; ++i) {
			if (!vis[i] && dist[i] < best) {
				best = dist[i];
				u = i;
			}
		}
		if (u == -1) break; // no hay más nodos alcanzables

		vis[u] = true;

		// procesar aristas desde u
		for (int v = 0; v < numNodos; ++v) {
			if (matrizAdyacencia[u][v] != 0) {
				int w = matrizAdyacencia[u][v];
				if (dist[u] + w < dist[v]) {
					dist[v] = dist[u] + w;
					prev[v].clear();
					prev[v].push_back(u); // u es el único predecesor en camino mínimo a v
				}
				else if (dist[u] + w == dist[v]) {
					// múltiples caminos mínimos: u también lleva a v con la misma distancia
					prev[v].push_back(u);
				}
			}
		}
	}

	// Si no hay camino a t
	if (dist[t] >= INF / 2) {
		return resultado; // sin camino
	}

	// Reconstruir TODOS los caminos mínimos desde s a t usando backtracking
	resultado.distancia = dist[t];
	std::vector<int> camino_actual;
	camino_actual.push_back(t);

	std::function<void(int)> construir_caminos = [&](int nodo) {
		if (nodo == s) {
			// llegamos al origen: invertir y guardar el camino
			std::vector<int> camino = camino_actual;
			reverse(camino.begin(), camino.end());
			resultado.caminos.push_back(camino);
			return;
		}
		// iterar sobre todos los predecesores de nodo en caminos mínimos
		for (int p : prev[nodo]) {
			camino_actual.push_back(p);
			construir_caminos(p);
			camino_actual.pop_back();
		}
		};

	construir_caminos(t);

	return resultado;
}

void Grafo::GenerarPosiciones(int n, double asp) {
	posiciones.clear();
	if (n <= 0) return;
	if (asp <= 0) asp = 1.0;
	aspecto = asp;

	double dmin = 0.8 * sqrt(asp / n);
	int fallos = 0;
	while ((int)posiciones.size() < n) {
		double x = asp * (rand() / (double)RAND_MAX);
		double y = rand() / (double)RAND_MAX;
		bool libre = true;
		for (size_t k = 0; k < posiciones.size(); k++) {
			double dx = x - posiciones[k].first;
			double dy = y - posiciones[k].second;
			if (dx * dx + dy * dy < dmin * dmin) { libre = false; break; }
		}
		if (libre) {
			posiciones.push_back(std::make_pair(x, y));
			fallos = 0;
		}
		else if (++fallos > 200) {
			dmin *= 0.9;
			fallos = 0;
		}
	}
	std::sort(posiciones.begin(), posiciones.end());
}

std::vector<PasoDijkstra> Grafo::ObtenerPasosDijkstra(int s, int t) {
	std::vector<PasoDijkstra> pasos;
	if (s < 0 || s >= numNodos || t < 0 || t >= numNodos || s == t) return pasos;

	const int n = numNodos;
	std::vector<int> dist(n, INF_DIJ), pred(n, -1), iterEt(n, -1);
	std::vector<bool> vis(n, false);
	dist[s] = 0;
	iterEt[s] = 0;

	auto foto = [&](const std::string& msg, int activo, int it) {
		PasoDijkstra p;
		p.mensaje = msg; p.nodoActivo = activo; p.iteracion = it;
		p.visitados = vis; p.dist = dist; p.pred = pred; p.iterEtiqueta = iterEt;
		return p;
		};
	auto txtPred = [&](int v) { return pred[v] == -1 ? std::string("-") : std::to_string(pred[v]); };

	{
		std::ostringstream m;
		m << "Vertice origen = " << s << "\r\n"
			<< "Etiqueta del origen: [0,-](0)";
		pasos.push_back(foto(m.str(), s, 0));
	}

	int it = 0;
	for (int k = 0; k < n; ++k) {
		++it;

		int u = -1, mejor = INF_DIJ;
		for (int j = 0; j < n; ++j)
			if (!vis[j] && dist[j] < mejor) { mejor = dist[j]; u = j; }
		if (u == -1) break;
		vis[u] = true;

		{
			std::ostringstream m;
			m << "Seleccion del vertice con menor distancia (iteracion " << it << ")\r\n"
				<< "Vertice " << u << " con d = " << dist[u] << "\r\n"
				<< "Etiqueta PERMANENTE: [" << dist[u] << "," << txtPred(u) << "](" << iterEt[u] << ")";
			pasos.push_back(foto(m.str(), u, it));
		}

		std::ostringstream m;
		m << "Vecinos de " << u << " (iteracion " << it << ")\r\n\r\n";
		std::vector<std::pair<int, int>> evaluadas;

		for (int v = 0; v < n; ++v) {
			int w = matrizAdyacencia[u][v];
			if (w == 0 || vis[v]) continue;
			int nueva = dist[u] + w;
			evaluadas.push_back(std::make_pair(u, v));

			m << "Arista " << u << " -> " << v << " (peso " << w << ")\r\n"
				<< "Nueva distancia: " << dist[u] << " + " << w << " = " << nueva << "\r\n";

			if (nueva < dist[v]) {
				m << "[ACTUALIZAR] Nueva etiqueta [" << nueva << "," << u << "](" << it << ")\r\n\r\n";
				dist[v] = nueva; pred[v] = u; iterEt[v] = it;
			}
			else if (nueva == dist[v]) {
				m << "[EMPATE] Mismo costo: tambien se llega por " << u << "\r\n\r\n";
			}
			else {
				m << "[CONSERVAR] Se mantiene la etiqueta [" << dist[v] << "," << txtPred(v) << "]\r\n\r\n";
			}
		}
		if (!evaluadas.empty()) {
			PasoDijkstra p = foto(m.str(), u, it);
			p.aristasEvaluadas = evaluadas;
			pasos.push_back(p);
		}
	}

	
	CaminosMinimos r = ObtenerCaminosMinimos(s, t);
	std::ostringstream m;
	m << " Determinacion de rutas minimas\r\n\r\n";
	if (r.caminos.empty()) {
		m << "No existe camino de " << s << " a " << t << ".";
	}
	else {
		m << "Distancia minima: " << r.distancia << "\r\n"
			<< "Cantidad de caminos minimos: " << r.caminos.size() << "\r\n\r\n";
		for (size_t c = 0; c < r.caminos.size(); ++c) {
			m << "Camino " << (c + 1) << ": ";
			for (size_t i = 0; i < r.caminos[c].size(); ++i) {
				m << r.caminos[c][i];
				if (i + 1 < r.caminos[c].size()) m << " -> ";
			}
			m << "\r\n";
		}
	}
	PasoDijkstra f = foto(m.str(), t, it);
	f.esFinal = true;
	pasos.push_back(f);
	return pasos;
}