#pragma once
#include "Animacion_funcionalidad.h"
namespace TrabajoFinalMateComputacionalAlgoritmoDijkstra2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de Como_funciona
	/// </summary>
	public ref class Como_funciona : public System::Windows::Forms::Form
	{
	public:
		Como_funciona(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
			bmpFondo_instrucciones = gcnew Bitmap("Imagenes/Instrucciones/sprite_fondo.png");
			instrucciones = gcnew Fondo_Instrucciones();
			g1 = this->CreateGraphics();
			canvas = BufferedGraphicsManager::Current;
			buffer = canvas->Allocate(g1, this->ClientRectangle);
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~Como_funciona()
		{
			if (timer1 != nullptr) timer1->Stop();
			if (buffer != nullptr) delete buffer;
			if (g1 != nullptr) delete g1;
			if (components) delete components;
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^ components;
		   Fondo_Instrucciones^ instrucciones;
		   Graphics^ g1;
		   BufferedGraphicsContext^ canvas;
		   BufferedGraphics^ buffer;
	protected:
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Panel^ panel1;

	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;

	protected:

	protected:

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>

		Bitmap^ bmpFondo_instrucciones;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Como_funciona::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Como_funciona::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->Location = System::Drawing::Point(4, 5);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(448, 29);
			this->label1->TabIndex = 0;
			this->label1->Text = L"<<¿CÓMO FUNCIONA DIJKSTRA\?>>";
			// 
			// label2
			// 
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Bold));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->Location = System::Drawing::Point(8, 40);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(640, 330);
			this->label2->TabIndex = 1;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::LightCyan;
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Location = System::Drawing::Point(472, 39);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(657, 380);
			this->panel1->TabIndex = 3;
			// 
			// label5
			// 
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Bold));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label5->Location = System::Drawing::Point(8, 40);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(640, 330);
			this->label5->TabIndex = 4;
			this->label5->Text = resources->GetString(L"label5.Text");
			// 
			// Como_funciona
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1141, 548);
			this->Controls->Add(this->panel1);
			this->DoubleBuffered = true;
			this->Name = L"Como_funciona";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"¿Cómo funciona\?";
			this->Load += gcnew System::EventHandler(this, &Como_funciona::Como_funciona_Load_1);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion



	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void Como_funciona_Load(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void Como_funciona_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		if (this->IsDisposed || !this->IsHandleCreated || this->timer1 == nullptr || !this->timer1->Enabled)
			return;
		if (bmpFondo_instrucciones == nullptr) return;
		// Crear bitmap temporal del tamaño del formulario
		Bitmap^ backBmp = gcnew Bitmap(this->ClientSize.Width, this->ClientSize.Height);
		Graphics^ gBack = Graphics::FromImage(backBmp);
		gBack->Clear(Color::WhiteSmoke);
		instrucciones->mover(gBack);
		instrucciones->dibujar(gBack, bmpFondo_instrucciones);

		buffer->Graphics->Clear(Color::WhiteSmoke);
		buffer->Graphics->DrawImage(backBmp, Point(0, 0));

		System::Drawing::Rectangle labelRect1 = System::Drawing::Rectangle(this->label1->Location, this->label1->Size);
		System::Drawing::Rectangle clientRect = System::Drawing::Rectangle(Point(0, 0), this->ClientSize);
		labelRect1.Intersect(clientRect);

		System::Drawing::Rectangle labelRect2 = System::Drawing::Rectangle(this->label2->Location, this->label2->Size);
		labelRect2.Intersect(clientRect);



		buffer->Render(gBack);
		buffer->Render(g1);

		// Limpiar recursos temporales
		delete gBack;
		delete backBmp;
	}

	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void panel2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void Como_funciona_Load_1(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}