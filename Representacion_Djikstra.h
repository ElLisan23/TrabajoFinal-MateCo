#pragma once
#include "Algoritmo_Djikstra.h"
#include <climits>
#include <queue>
#include <limits>
#include <sstream>

namespace TrabajoFinalMateComputacionalAlgoritmoDijkstra2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace Microsoft::VisualBasic;

	/// <summary>
	/// Resumen de Representacion_Dijkstra
	/// </summary>
	public ref class Representacion_Dijkstra : public System::Windows::Forms::Form
	{
	private:
		Grafo* grafo;
		int origen = -1;
		int destino = -1;
		std::vector<PasoDijkstra>* pasos;
		int pasoActual = -1;
		System::Windows::Forms::Button^ btnAnterior;
		System::Windows::Forms::Button^ btnSiguiente;
		// Animacion de creacion de nodos
		int visibleNodes = 0;
		System::Windows::Forms::Timer^ nodeTimer;

		// Para resaltar aristas del resultado de Dijkstra
		System::Collections::Generic::List<System::Tuple<int, int>^>^ highlightedEdges;

		// Para guardar múltiples caminos con colores asociados
		System::Collections::Generic::List<System::Drawing::Color>^ pathColors;
		System::Collections::Generic::List<System::Collections::Generic::List<System::Tuple<int, int>^>^>^ allHighlightedPaths;

		// Para animacion de aristas y control de visibilidad (managed)
		array<array<bool>^>^ visibleEdges;
		System::Collections::Generic::List<System::Tuple<int, int>^>^ pendingEdges;
		int edgeRevealIndex = 0;
		bool animatingEdges = false;

		// Controles para Dijkstra (panel derecho)
		System::Windows::Forms::ComboBox^ cmbDijOrigen;
		System::Windows::Forms::ComboBox^ cmbDijDestino;
		System::Windows::Forms::Button^ btnRunDijkstra;
		System::Windows::Forms::TextBox^ txtDijkstraInfo;
		System::Windows::Forms::Label^ lblDijOrigen;
		System::Windows::Forms::Label^ lblDijDestino;
	public:
		Representacion_Dijkstra(void)
		{
			InitializeComponent();
			// Inicializamos el puntero de nuestro grafo
			srand((unsigned)time(nullptr)); //Para aleatorizar GG
			grafo = new Grafo();
			CrearControlesPaso();

			this->DoubleBuffered = true;
			this->SetStyle(ControlStyles::OptimizedDoubleBuffer |
				ControlStyles::UserPaint |
				ControlStyles::AllPaintingInWmPaint, true);
			this->UpdateStyles();
		}

		// Permite seleccionar modo desde otra ventana
		void SetMode(bool manual) {
			this->rbManual->Checked = manual;
			this->rbAleatorio->Checked = !manual;
			// ocultar selección de modo porque ya fue elegida
			this->rbManual->Visible = false;
			this->rbAleatorio->Visible = false;
			this->lblMode->Visible = true;
			this->lblMode->Text = manual ? "Modo: Manual" : "Modo: Aleatorio";
			// mostrar indicación para generar desde matriz si es manual
			this->lblMatrixHint->Visible = manual;
		}

		// Ejecutar Dijkstra y mostrar información del camino mínimo (solo el camino, sin iteraciones)
	private: System::Void btnRunDijkstra_Click(System::Object^ sender, System::EventArgs^ e) {
		int n = grafo->ObtenerNumNodos();
		if (n == 0) { MessageBox::Show("Grafo vacío."); return; }
		int s = this->cmbDijOrigen->SelectedIndex;
		int t = this->cmbDijDestino->SelectedIndex;
		if (s == -1 || t == -1) { MessageBox::Show("Selecciona origen y destino para Dijkstra."); return; }
		if (s == t) { MessageBox::Show("Origen y destino deben ser distintos."); return; }

		this->origen = s;
		this->destino = t;
		*pasos = grafo->ObtenerPasosDijkstra(s, t);
		this->pasoActual = 0;
		MostrarPaso();
	}

	private: void ResaltarCaminos(const CaminosMinimos& resultado) {
		this->pathColors = gcnew System::Collections::Generic::List<System::Drawing::Color>();
		this->allHighlightedPaths = gcnew System::Collections::Generic::List<System::Collections::Generic::List<System::Tuple<int, int>^>^>();
		array<System::Drawing::Color>^ colors = gcnew array<System::Drawing::Color>{
			System::Drawing::Color::Red, System::Drawing::Color::Blue, System::Drawing::Color::Green,
			System::Drawing::Color::Orange, System::Drawing::Color::Purple };
		for (size_t c = 0; c < resultado.caminos.size(); ++c) {
			auto camino_edges = gcnew System::Collections::Generic::List<System::Tuple<int, int>^>();
			for (size_t i = 1; i < resultado.caminos[c].size(); ++i)
				camino_edges->Add(System::Tuple::Create(resultado.caminos[c][i - 1], resultado.caminos[c][i]));
			this->allHighlightedPaths->Add(camino_edges);
			this->pathColors->Add(colors[c % colors->Length]);
		}
	}

		   // Muestra un cuadro de diálogo simple para pedir una cadena al usuario.
	private: String^ PromptInput(String^ message, String^ title, String^ defaultVal) {
		Form^ f = gcnew Form();
		f->Text = title;
		f->StartPosition = FormStartPosition::CenterParent;
		f->Size = System::Drawing::Size(320, 150);
		f->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
		f->MaximizeBox = false;
		f->MinimizeBox = false;

		Label^ lbl = gcnew Label();
		lbl->Text = message;
		lbl->AutoSize = true;
		lbl->Location = System::Drawing::Point(10, 10);
		f->Controls->Add(lbl);

		TextBox^ tb = gcnew TextBox();
		tb->Text = defaultVal;
		tb->Location = System::Drawing::Point(10, 40);
		tb->Width = 280;
		f->Controls->Add(tb);

		Button^ ok = gcnew Button();
		ok->Text = "OK";
		ok->DialogResult = System::Windows::Forms::DialogResult::OK;
		ok->Location = System::Drawing::Point(80, 75);
		f->Controls->Add(ok);

		Button^ cancel = gcnew Button();
		cancel->Text = "Cancelar";
		cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		cancel->Location = System::Drawing::Point(170, 75);
		f->Controls->Add(cancel);

		f->AcceptButton = ok;
		f->CancelButton = cancel;

		if (f->ShowDialog() == System::Windows::Forms::DialogResult::OK) return tb->Text; else return nullptr;
	}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~Representacion_Dijkstra()
		{

			if (components)
			{
				delete pasos;
				delete grafo;
				delete components;
			}
		}

		// Timer tick para animar la aparicion de nodos
	private: System::Void nodeTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
		int n = grafo->ObtenerNumNodos();
		if (visibleNodes < n) {
			visibleNodes++;
			this->Panel_Grafo->Refresh();
		}
		else {
			// empezar a revelar aristas si hay
			if (!animatingEdges) {
				animatingEdges = true;
				this->edgeRevealIndex = 0;
				// si pendingEdges vacío, construirlo desde la matriz
				if (this->pendingEdges == nullptr || this->pendingEdges->Count == 0) {
					const std::vector<std::vector<int>>& mat = grafo->ObtenerMatriz();
					if (this->pendingEdges == nullptr) this->pendingEdges = gcnew System::Collections::Generic::List<System::Tuple<int, int>^>();
					for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (mat[i][j] != 0) this->pendingEdges->Add(System::Tuple::Create(i, j));
				}
				// si no hay aristas, terminar anim
				if (this->pendingEdges == nullptr || this->pendingEdges->Count == 0) {
					this->nodeTimer->Stop();
					// llenar solo los combos de Dijkstra (panel derecho)
					this->cmbDijOrigen->Items->Clear();
					this->cmbDijDestino->Items->Clear();
					for (int i = 0; i < n; ++i) { this->cmbDijOrigen->Items->Add(i.ToString()); this->cmbDijDestino->Items->Add(i.ToString()); }
					return;
				}
			}
			// revelar siguiente arista
			if (animatingEdges && this->pendingEdges != nullptr && edgeRevealIndex < this->pendingEdges->Count) {
				System::Tuple<int, int>^ pr = this->pendingEdges[edgeRevealIndex++];
				int u = pr->Item1; int v = pr->Item2;
				if (this->visibleEdges != nullptr && this->visibleEdges->Length == n) this->visibleEdges[u][v] = true;
				this->Panel_Grafo->Refresh();
				return;
			}
			// terminado
			this->nodeTimer->Stop();
			// llenar solo los combos del panel derecho (Dijkstra)
			this->cmbDijOrigen->Items->Clear();
			this->cmbDijDestino->Items->Clear();
			for (int i = 0; i < n; ++i) { this->cmbDijOrigen->Items->Add(i.ToString()); this->cmbDijDestino->Items->Add(i.ToString()); }
		}
	}

	private: System::Windows::Forms::NumericUpDown^ txtNodos;
	private: System::Windows::Forms::Button^ Btn_Generar_Grafo;
	private: System::Windows::Forms::RadioButton^ rbAleatorio;
	private: System::Windows::Forms::RadioButton^ rbManual;
	private: System::Windows::Forms::TextBox^ txtManualInput;
	private: System::Windows::Forms::Panel^ Panel_Grafo;
	private: array<System::Drawing::Point>^ nodosPos;
	private: int radioNodoMember = 18;
	private: int seleccionadoOrigenClick = -1;
	private: System::Windows::Forms::Label^ lblAviso;
		   // Left-side origin/destination controls removed (Dijkstra controls are on the right)
	private: System::Windows::Forms::Label^ lblMode;
	private: System::Windows::Forms::Label^ lblMatrixHint;
	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->txtNodos = (gcnew System::Windows::Forms::NumericUpDown());
			this->Btn_Generar_Grafo = (gcnew System::Windows::Forms::Button());
			this->rbAleatorio = (gcnew System::Windows::Forms::RadioButton());
			this->rbManual = (gcnew System::Windows::Forms::RadioButton());
			this->txtManualInput = (gcnew System::Windows::Forms::TextBox());
			this->Panel_Grafo = (gcnew System::Windows::Forms::Panel());
			this->lblAviso = (gcnew System::Windows::Forms::Label());
			this->lblMatrixHint = (gcnew System::Windows::Forms::Label());
			this->lblMode = (gcnew System::Windows::Forms::Label());
			this->nodeTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->cmbDijOrigen = (gcnew System::Windows::Forms::ComboBox());
			this->cmbDijDestino = (gcnew System::Windows::Forms::ComboBox());
			this->btnRunDijkstra = (gcnew System::Windows::Forms::Button());
			this->txtDijkstraInfo = (gcnew System::Windows::Forms::TextBox());
			this->lblDijOrigen = (gcnew System::Windows::Forms::Label());
			this->lblDijDestino = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->txtNodos))->BeginInit();
			this->SuspendLayout();
			// 
			// txtNodos
			// 
			this->txtNodos->Location = System::Drawing::Point(20, 86);
			this->txtNodos->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->txtNodos->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 16, 0, 0, 0 });
			this->txtNodos->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 7, 0, 0, 0 });
			this->txtNodos->Name = L"txtNodos";
			this->txtNodos->Size = System::Drawing::Size(90, 20);
			this->txtNodos->TabIndex = 1;
			this->txtNodos->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 7, 0, 0, 0 });
			this->txtNodos->ValueChanged += gcnew System::EventHandler(this, &Representacion_Dijkstra::txtNodos_ValueChanged);
			// 
			// Btn_Generar_Grafo
			// 
			this->Btn_Generar_Grafo->Location = System::Drawing::Point(34, 135);
			this->Btn_Generar_Grafo->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Btn_Generar_Grafo->Name = L"Btn_Generar_Grafo";
			this->Btn_Generar_Grafo->Size = System::Drawing::Size(56, 19);
			this->Btn_Generar_Grafo->TabIndex = 2;
			this->Btn_Generar_Grafo->Text = L"Presioname";
			this->Btn_Generar_Grafo->UseVisualStyleBackColor = true;
			this->Btn_Generar_Grafo->Click += gcnew System::EventHandler(this, &Representacion_Dijkstra::Btn_Generar_Grafo_Click);
			// 
			// rbAleatorio
			// 
			this->rbAleatorio->AutoSize = true;
			this->rbAleatorio->Checked = true;
			this->rbAleatorio->Location = System::Drawing::Point(20, 32);
			this->rbAleatorio->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rbAleatorio->Name = L"rbAleatorio";
			this->rbAleatorio->Size = System::Drawing::Size(66, 17);
			this->rbAleatorio->TabIndex = 0;
			this->rbAleatorio->TabStop = true;
			this->rbAleatorio->Text = L"Aleatorio";
			this->rbAleatorio->UseVisualStyleBackColor = true;
			// 
			// rbManual
			// 
			this->rbManual->AutoSize = true;
			this->rbManual->Location = System::Drawing::Point(20, 49);
			this->rbManual->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->rbManual->Name = L"rbManual";
			this->rbManual->Size = System::Drawing::Size(60, 17);
			this->rbManual->TabIndex = 0;
			this->rbManual->TabStop = true;
			this->rbManual->Text = L"Manual";
			this->rbManual->UseVisualStyleBackColor = true;
			this->rbManual->CheckedChanged += gcnew System::EventHandler(this, &Representacion_Dijkstra::rbManual_CheckedChanged);
			// 
			// txtManualInput
			// 
			this->txtManualInput->AcceptsTab = true;
			this->txtManualInput->Location = System::Drawing::Point(9, 292);
			this->txtManualInput->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->txtManualInput->Multiline = true;
			this->txtManualInput->Name = L"txtManualInput";
			this->txtManualInput->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtManualInput->Size = System::Drawing::Size(213, 201);
			this->txtManualInput->TabIndex = 10;
			this->txtManualInput->Visible = false;
			this->txtManualInput->WordWrap = false;
			// 
			// Panel_Grafo
			// 
			this->Panel_Grafo->Location = System::Drawing::Point(226, 10);
			this->Panel_Grafo->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Panel_Grafo->Name = L"Panel_Grafo";
			this->Panel_Grafo->Size = System::Drawing::Size(574, 483);
			this->Panel_Grafo->TabIndex = 3;
			this->Panel_Grafo->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Representacion_Dijkstra::Panel_Grafo_Paint);
			this->Panel_Grafo->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Representacion_Dijkstra::Panel_Grafo_MouseClick);
			// 
			// lblAviso
			// 
			this->lblAviso->AutoSize = true;
			this->lblAviso->Location = System::Drawing::Point(18, 69);
			this->lblAviso->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->lblAviso->Name = L"lblAviso";
			this->lblAviso->Size = System::Drawing::Size(204, 13);
			this->lblAviso->TabIndex = 0;
			this->lblAviso->Text = L"Mínimo permitido: 7, Máximo permitido: 16";
			// 
			// lblMatrixHint
			// 
			this->lblMatrixHint->AutoSize = true;
			this->lblMatrixHint->Location = System::Drawing::Point(9, 272);
			this->lblMatrixHint->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->lblMatrixHint->Name = L"lblMatrixHint";
			this->lblMatrixHint->Size = System::Drawing::Size(169, 13);
			this->lblMatrixHint->TabIndex = 0;
			this->lblMatrixHint->Text = L"Generar el grafo a traves de matriz";
			this->lblMatrixHint->Visible = false;
			// 
			// lblMode
			// 
			this->lblMode->AutoSize = true;
			this->lblMode->Location = System::Drawing::Point(20, 10);
			this->lblMode->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->lblMode->Name = L"lblMode";
			this->lblMode->Size = System::Drawing::Size(0, 13);
			this->lblMode->TabIndex = 1;
			this->lblMode->Visible = false;
			// 
			// nodeTimer
			// 
			this->nodeTimer->Interval = 250;
			this->nodeTimer->Tick += gcnew System::EventHandler(this, &Representacion_Dijkstra::nodeTimer_Tick);
			// 
			// cmbDijOrigen
			// 
			this->cmbDijOrigen->Location = System::Drawing::Point(825, 32);
			this->cmbDijOrigen->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cmbDijOrigen->Name = L"cmbDijOrigen";
			this->cmbDijOrigen->Size = System::Drawing::Size(114, 21);
			this->cmbDijOrigen->TabIndex = 11;
			// 
			// cmbDijDestino
			// 
			this->cmbDijDestino->Location = System::Drawing::Point(825, 85);
			this->cmbDijDestino->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->cmbDijDestino->Name = L"cmbDijDestino";
			this->cmbDijDestino->Size = System::Drawing::Size(114, 21);
			this->cmbDijDestino->TabIndex = 12;
			// 
			// btnRunDijkstra
			// 
			this->btnRunDijkstra->Location = System::Drawing::Point(825, 123);
			this->btnRunDijkstra->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->btnRunDijkstra->Name = L"btnRunDijkstra";
			this->btnRunDijkstra->Size = System::Drawing::Size(112, 24);
			this->btnRunDijkstra->TabIndex = 13;
			this->btnRunDijkstra->Text = L"Ejecutar Dijkstra";
			this->btnRunDijkstra->Click += gcnew System::EventHandler(this, &Representacion_Dijkstra::btnRunDijkstra_Click);
			// 
			// txtDijkstraInfo
			// 
			this->txtDijkstraInfo->Location = System::Drawing::Point(825, 151);
			this->txtDijkstraInfo->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->txtDijkstraInfo->Multiline = true;
			this->txtDijkstraInfo->Name = L"txtDijkstraInfo";
			this->txtDijkstraInfo->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtDijkstraInfo->Size = System::Drawing::Size(226, 342);
			this->txtDijkstraInfo->TabIndex = 14;
			// 
			// lblDijOrigen
			// 
			this->lblDijOrigen->AutoSize = true;
			this->lblDijOrigen->Location = System::Drawing::Point(825, 16);
			this->lblDijOrigen->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->lblDijOrigen->Name = L"lblDijOrigen";
			this->lblDijOrigen->Size = System::Drawing::Size(119, 13);
			this->lblDijOrigen->TabIndex = 15;
			this->lblDijOrigen->Text = L"Vértice origen (Dijkstra):";
			// 
			// lblDijDestino
			// 
			this->lblDijDestino->AutoSize = true;
			this->lblDijDestino->Location = System::Drawing::Point(825, 69);
			this->lblDijDestino->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->lblDijDestino->Name = L"lblDijDestino";
			this->lblDijDestino->Size = System::Drawing::Size(124, 13);
			this->lblDijDestino->TabIndex = 16;
			this->lblDijDestino->Text = L"Vértice destino (Dijkstra):";
			// 
			// Representacion_Dijkstra
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1057, 531);
			this->Controls->Add(this->lblMatrixHint);
			this->Controls->Add(this->lblMode);
			this->Controls->Add(this->lblAviso);
			this->Controls->Add(this->Panel_Grafo);
			this->Controls->Add(this->Btn_Generar_Grafo);
			this->Controls->Add(this->txtNodos);
			this->Controls->Add(this->rbAleatorio);
			this->Controls->Add(this->rbManual);
			this->Controls->Add(this->txtManualInput);
			this->Controls->Add(this->cmbDijOrigen);
			this->Controls->Add(this->cmbDijDestino);
			this->Controls->Add(this->btnRunDijkstra);
			this->Controls->Add(this->txtDijkstraInfo);
			this->Controls->Add(this->lblDijOrigen);
			this->Controls->Add(this->lblDijDestino);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Representacion_Dijkstra";
			this->Text = L"Representacion_Dijkstra";
			this->Load += gcnew System::EventHandler(this, &Representacion_Dijkstra::Representacion_Dijkstra_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->txtNodos))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void txtNodos_ValueChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void Btn_Generar_Grafo_Click(System::Object^ sender, System::EventArgs^ e) {
		ReiniciarPasos();
		int n = safe_cast<int>(this->txtNodos->Value);
		if (this->rbManual->Checked) {
			// Leer matriz desde el textbox (cada fila en una línea, valores separados por espacios)
			String^ texto = this->txtManualInput->Text->Trim();
			if (String::IsNullOrWhiteSpace(texto)) {
				// Si el textbox está vacío, creamos un grafo sin aristas y animamos la aparición de nodos
				std::vector<std::vector<int>> mat(n, std::vector<int>(n, 0));
				grafo->CargarDesdeMatriz(mat);
				this->visibleNodes = 0;
				this->visibleEdges = gcnew array<array<bool>^>(n);
				for (int ii = 0; ii < n; ++ii) { this->visibleEdges[ii] = gcnew array<bool>(n); for (int jj = 0; jj < n; ++jj) this->visibleEdges[ii][jj] = false; }
				this->pendingEdges = gcnew System::Collections::Generic::List<System::Tuple<int, int>^>();
				this->edgeRevealIndex = 0;
				this->animatingEdges = false;
				this->nodeTimer->Start();
				MessageBox::Show("Grafo vacío creado. Usa los clicks sobre los nodos para añadir aristas sin crear ciclos.");
				return;
			}
			else {
				array<String^>^ filas = texto->Split(gcnew array<wchar_t>{ '\n' }, StringSplitOptions::RemoveEmptyEntries);
				if ((int)filas->Length != n) {
					MessageBox::Show("La cantidad de filas no coincide con el número de nodos.");
					return;
				}
				std::vector<std::vector<int>> mat(n, std::vector<int>(n, 0));
				for (int i = 0; i < n; i++) {
					String^ linea = filas[i]->Trim();
					array<String^>^ tokens = linea->Split(gcnew array<wchar_t>{ ' ', '\t', ',' }, StringSplitOptions::RemoveEmptyEntries);
					if ((int)tokens->Length != n) {
						MessageBox::Show("Cada fila debe contener exactamente " + n.ToString() + " valores.");
						return;
					}
					for (int j = 0; j < n; j++) {
						try {
							int val = Int32::Parse(tokens[j]);
							mat[i][j] = val;
						}
						catch (...) {
							MessageBox::Show("Valor inválido en la matriz. Usa enteros.");
							return;
						}
					}
				}

				if (!grafo->CargarDesdeMatriz(mat)) {
					MessageBox::Show("La matriz contiene ciclos o no es válida. El grafo debe ser acíclico.");
					return;
				}
				this->visibleNodes = n;
				// marcar todas las aristas como visibles directamente (managed)
				this->visibleEdges = gcnew array<array<bool>^>(n);
				for (int ii = 0; ii < n; ++ii) { this->visibleEdges[ii] = gcnew array<bool>(n); for (int jj = 0; jj < n; ++jj) this->visibleEdges[ii][jj] = (mat[ii][jj] != 0); }
			}

		}
		else {
			grafo->GenerarAleatorio(n);
			const std::vector<std::vector<int>>& mat = grafo->ObtenerMatriz();
			System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					sb->Append(System::Convert::ToString(mat[i][j]));
					if (j < n - 1) sb->Append(" ");
				}
				if (i < n - 1) sb->AppendLine();
			}
			this->txtManualInput->Text = sb->ToString();
			this->txtManualInput->Visible = true;
			// mostrar nodos inmediatamente, preparar animacion solo para aristas
			this->visibleNodes = n; // no parpadeo de nodos
			this->visibleEdges = gcnew array<array<bool>^>(n);
			for (int ii = 0; ii < n; ++ii) { this->visibleEdges[ii] = gcnew array<bool>(n); for (int jj = 0; jj < n; ++jj) this->visibleEdges[ii][jj] = false; }
			this->pendingEdges = gcnew System::Collections::Generic::List<System::Tuple<int, int>^>();
			const std::vector<std::vector<int>>& mat2 = grafo->ObtenerMatriz();
			for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (mat2[i][j] != 0) this->pendingEdges->Add(System::Tuple::Create(i, j));
			this->edgeRevealIndex = 0;
			this->animatingEdges = true; // empezar a revelar aristas
			this->nodeTimer->Start();

			this->cmbDijOrigen->Items->Clear();
			this->cmbDijDestino->Items->Clear();
			for (int i = 0; i < n; ++i) { this->cmbDijOrigen->Items->Add(i.ToString()); this->cmbDijDestino->Items->Add(i.ToString()); }
		}

		Panel_Grafo->Refresh();

		this->cmbDijOrigen->Items->Clear();
		this->cmbDijDestino->Items->Clear();
		for (int i = 0; i < n; i++) {
			this->cmbDijOrigen->Items->Add(i.ToString());
			this->cmbDijDestino->Items->Add(i.ToString());
		}

		MessageBox::Show("¡Grafo generado con éxito!");
	}

	private: System::Void rbManual_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->txtManualInput->Visible = this->rbManual->Checked;
		if (this->rbManual->Checked) {
			// Al pasar a modo manual, borrar grafo existente (si lo había)
			ReiniciarPasos();
			this->grafo->Vaciar();
			this->Panel_Grafo->Refresh();
			this->origen = -1;
			this->destino = -1;
		}
	}
	private: System::Void Panel_Grafo_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		int n = grafo->ObtenerNumNodos();
		if (n == 0) return;
		const std::vector<std::vector<int>>& matriz = grafo->ObtenerMatriz();
		const PasoDijkstra* paso = nullptr;
		if (pasos != nullptr && pasoActual >= 0 && pasoActual < (int)pasos->size())
			paso = &(*pasos)[pasoActual];
		Graphics^ g = e->Graphics;
		g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

		int radioNodo = 18;

		array<Point>^ pos = gcnew array<Point>(n);
		if (grafo->TienePosiciones()) {
			int margen = 40;
			double asp = grafo->ObtenerAspecto();
			double escala = Math::Min((Panel_Grafo->ClientSize.Width - 2 * margen) / asp, (double) Panel_Grafo->ClientSize.Height - 2 * margen);
			for (int i = 0; i < n; i++) {
				std::pair<double, double> p = grafo->ObtenerPos(i);
				pos[i] = Point(margen + (int)(p.first * escala), margen + (int)(p.second * escala));
			}
		}
		else {
			return; // modo manual anexado por el momento
		}


		this->nodosPos = pos;
		this->radioNodoMember = radioNodo;

		Pen^ penArista = gcnew Pen(Color::DimGray, 2);
		penArista->CustomEndCap = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(5, 6);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (matriz[i][j] != 0) {

					if (this->visibleEdges != nullptr && this->visibleEdges->Length == n) {
						if (!this->visibleEdges[i][j]) continue;
					}

					if (visibleNodes > 0 && (i >= visibleNodes || j >= visibleNodes)) continue;
					double dx = pos[j].X - pos[i].X;
					double dy = pos[j].Y - pos[i].Y;
					double distancia = Math::Sqrt(dx * dx + dy * dy);

					if (distancia > 0) {
						int startX = pos[i].X + (int)((dx / distancia) * radioNodo);
						int startY = pos[i].Y + (int)((dy / distancia) * radioNodo);

						int endX = pos[j].X - (int)((dx / distancia) * radioNodo);
						int endY = pos[j].Y - (int)((dy / distancia) * radioNodo);

						Point pInicio(startX, startY);
						Point pFin(endX, endY);

						// buscar si esta arista está en alguno de los caminos resaltados
						int colorIndex = -1;
						if (this->allHighlightedPaths != nullptr) {
							for (int pathIdx = 0; pathIdx < this->allHighlightedPaths->Count; ++pathIdx) {
								System::Collections::Generic::List<System::Tuple<int, int>^>^ camino = this->allHighlightedPaths[pathIdx];
								for each (System::Tuple<int, int> ^ t in camino) {
									if (t->Item1 == i && t->Item2 == j) { colorIndex = pathIdx; break; }
								}
								if (colorIndex >= 0) break;
							}
						}
						bool evaluando = false;
						if (paso != nullptr)
							for (size_t a = 0; a < paso->aristasEvaluadas.size(); ++a)
								if (paso->aristasEvaluadas[a].first == i && paso->aristasEvaluadas[a].second == j) evaluando = true;
						if (colorIndex >= 0) {
							// resaltar con color del camino
							System::Drawing::Color pathColor = this->pathColors[colorIndex];
							Pen^ penR = gcnew Pen(pathColor, 4);
							penR->CustomEndCap = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(6, 8);
							g->DrawLine(penR, pInicio, pFin);
						}
						else if (evaluando) {
							Pen^ penE = gcnew Pen(Color::DarkOrange, 3);
							penE->CustomEndCap = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(6, 8);
							g->DrawLine(penE, pInicio, pFin);
						}
						else {
							g->DrawLine(penArista, pInicio, pFin);
						}
						Point medio((pInicio.X + pFin.X) / 2, (pInicio.Y + pFin.Y) / 2);
						g->DrawString(matriz[i][j].ToString(), this->Font, Brushes::Red, medio);
					}
				}
			}
		}
		for (int i = 0; i < n; i++) {
			Rectangle r(pos[i].X - radioNodo, pos[i].Y - radioNodo, radioNodo * 2, radioNodo * 2);
			System::Drawing::Font^ fEt = gcnew System::Drawing::Font("Segoe UI", 7.5F, FontStyle::Bold);

			for (int i = 0; i < n; i++) {
				Rectangle r(pos[i].X - radioNodo, pos[i].Y - radioNodo, radioNodo * 2, radioNodo * 2);

				Brush^ relleno = Brushes::LightSkyBlue;
				if (this->seleccionadoOrigenClick == i) relleno = Brushes::LightGreen;
				if (paso != nullptr) {
					if (paso->esFinal && i == origen)       relleno = Brushes::Orange;
					else if (paso->esFinal && i == destino) relleno = Brushes::DeepSkyBlue;
					else if (paso->nodoActivo == i)         relleno = Brushes::Gold;        // procesándose ahora
					else if (paso->visitados[i])            relleno = Brushes::LightGreen;  // etiqueta permanente
				}
				g->FillEllipse(relleno, r);
				g->DrawEllipse(Pens::Black, r);

				String^ etq = i.ToString();
				SizeF tam = g->MeasureString(etq, this->Font);
				g->DrawString(etq, this->Font, Brushes::Black, pos[i].X - tam.Width / 2, pos[i].Y - tam.Height / 2);

				// Etiqueta [d,V](i) debajo del nodo
				if (paso != nullptr && paso->dist[i] < Grafo::INF_DIJ) {
					String^ et = "[" + Convert::ToString(paso->dist[i]) + "," +
						(paso->pred[i] == -1 ? "-" : Convert::ToString(paso->pred[i])) + "](" +
						Convert::ToString(paso->iterEtiqueta[i]) + ")";
					SizeF te = g->MeasureString(et, fEt);
					g->DrawString(et, fEt,
						paso->visitados[i] ? Brushes::DarkGreen : Brushes::DarkOrange,
						pos[i].X - te.Width / 2, pos[i].Y + radioNodo + 1);
				}
			}
		}
	}

	private: System::Void Panel_Grafo_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (!this->rbManual->Checked) return; // solo en modo manual
		int n = grafo->ObtenerNumNodos();
		if (n == 0) return;
		if (this->nodosPos == nullptr) return;
		// encontrar nodo clickeado
		int encontrado = -1;
		for (int i = 0; i < n; ++i) {
			Point p = this->nodosPos[i];
			double dx = e->X - p.X;
			double dy = e->Y - p.Y;
			double dist2 = dx * dx + dy * dy;
			double radio2 = (this->radioNodoMember * 1.5) * (this->radioNodoMember * 1.5);
			if (dist2 <= radio2) { encontrado = i; break; }
		}
		if (encontrado == -1) return;
		if (this->seleccionadoOrigenClick == -1) {
			// seleccionar origen
			this->seleccionadoOrigenClick = encontrado;
			this->Panel_Grafo->Refresh();
			return;
		}
		// ya había origen seleccionado
		int origenClick = this->seleccionadoOrigenClick;
		int destinoClick = encontrado;
		if (origenClick == destinoClick) {
			// deseleccionar
			this->seleccionadoOrigenClick = -1;
			this->Panel_Grafo->Refresh();
			return;
		}
		// pedir peso
		String^ input = this->PromptInput("Ingrese peso para la arista " + origenClick.ToString() + " -> " + destinoClick.ToString(), "Peso", "1");
		if (String::IsNullOrWhiteSpace(input)) {
			this->seleccionadoOrigenClick = -1;
			this->Panel_Grafo->Refresh();
			return;
		}
		int peso = 0;
		try {
			peso = Int32::Parse(input);
		}
		catch (...) {
			MessageBox::Show("Peso inválido. Usa un entero.");
			this->seleccionadoOrigenClick = -1;
			this->Panel_Grafo->Refresh();
			return;
		}
		// intentar agregar arista sin crear ciclo
		if (!grafo->AgregarAristaSiAcyclic(origenClick, destinoClick, peso)) {
			MessageBox::Show("No se puede agregar la arista: crearía un ciclo.");
			this->seleccionadoOrigenClick = -1;
			this->Panel_Grafo->Refresh();
			return;
		}
		// agregado correctamente
		ReiniciarPasos();
		 // marcar arista visible (si estructura inicializada)
		if (this->visibleEdges != nullptr && this->visibleEdges->Length == grafo->ObtenerNumNodos()) this->visibleEdges[origenClick][destinoClick] = true;
		this->seleccionadoOrigenClick = -1;
		this->Panel_Grafo->Refresh();
	}
	private: void CrearControlesPaso() {
		pasos = new std::vector<PasoDijkstra>();

		// el cuadro de la derecha ahora muestra el paso actual
		this->txtDijkstraInfo->Size = System::Drawing::Size(226, 300);
		this->txtDijkstraInfo->ReadOnly = true;
		this->txtDijkstraInfo->Font = gcnew System::Drawing::Font("Consolas", 8.25F);

		btnAnterior = gcnew Button();
		btnAnterior->Text = L"<< Anterior";
		btnAnterior->Location = System::Drawing::Point(825, 458);
		btnAnterior->Size = System::Drawing::Size(110, 28);
		btnAnterior->Enabled = false;
		btnAnterior->Click += gcnew System::EventHandler(this, &Representacion_Dijkstra::btnAnterior_Click);
		this->Controls->Add(btnAnterior);

		btnSiguiente = gcnew Button();
		btnSiguiente->Text = L"Siguiente >>";
		btnSiguiente->Location = System::Drawing::Point(941, 458);
		btnSiguiente->Size = System::Drawing::Size(110, 28);
		btnSiguiente->Enabled = false;
		btnSiguiente->Click += gcnew System::EventHandler(this, &Representacion_Dijkstra::btnSiguiente_Click);
		this->Controls->Add(btnSiguiente);
	}

private: System::Void btnSiguiente_Click(System::Object^ sender, System::EventArgs^ e) {
	if (pasoActual < (int)pasos->size() - 1) { pasoActual++; MostrarPaso(); }
}
private: System::Void btnAnterior_Click(System::Object^ sender, System::EventArgs^ e) {
	if (pasoActual > 0) { pasoActual--; MostrarPaso(); }
}

private: void ReiniciarPasos() {
	pasos->clear();
	pasoActual = -1;
	allHighlightedPaths = nullptr;
	pathColors = nullptr;
	btnAnterior->Enabled = false;
	btnSiguiente->Enabled = false;
	txtDijkstraInfo->Text = "";
}

private: void MostrarPaso() {
	if (pasoActual < 0 || pasoActual >= (int)pasos->size()) return;
	const PasoDijkstra& p = (*pasos)[pasoActual];

	std::stringstream ss;
	ss << "Paso " << (pasoActual + 1) << " de " << pasos->size() << "\r\n\r\n"
		<< p.mensaje << "\r\n\r\nEtiquetas [d,V](i):\r\n";
	for (size_t i = 0; i < p.dist.size(); ++i) {
		if (p.dist[i] >= Grafo::INF_DIJ) continue;
		ss << "  v" << i << ": [" << p.dist[i] << ","
			<< (p.pred[i] == -1 ? std::string("-") : std::to_string(p.pred[i]))
			<< "](" << p.iterEtiqueta[i] << ")" << (p.visitados[i] ? "  *permanente" : "") << "\r\n";
	}
	txtDijkstraInfo->Text = gcnew String(ss.str().c_str());

	btnAnterior->Enabled = (pasoActual > 0);
	btnSiguiente->Enabled = (pasoActual < (int)pasos->size() - 1);

	// Solo en el último paso se resaltan los caminos con colores
	if (p.esFinal) ResaltarCaminos(grafo->ObtenerCaminosMinimos(origen, destino));
	else { allHighlightedPaths = nullptr; pathColors = nullptr; }

	Panel_Grafo->Refresh();
}
	private: System::Void Representacion_Dijkstra_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}