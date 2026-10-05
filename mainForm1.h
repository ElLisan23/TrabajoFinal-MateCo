#pragma once
#include "Representacion_Djikstra.h"
#include "Como_funciona.h"

namespace TrabajoFinalMateComputacionalAlgoritmoDijkstra2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de mainForm
	/// </summary>
	public ref class mainForm : public System::Windows::Forms::Form
	{
	public:
		mainForm(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~mainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ txtTitle;
	private: System::Windows::Forms::Button^ btnManual;
	protected:


	private: System::Windows::Forms::Button^ btnAutomatico;

	private: System::Windows::Forms::Button^ btnComoFunciona;

	private: System::Windows::Forms::Button^ btnIntegrantes;
	private: System::Windows::Forms::GroupBox^ groupBox1;

	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>


		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txtTitle = (gcnew System::Windows::Forms::Label());
			this->btnManual = (gcnew System::Windows::Forms::Button());
			this->btnAutomatico = (gcnew System::Windows::Forms::Button());
			this->btnComoFunciona = (gcnew System::Windows::Forms::Button());
			this->btnIntegrantes = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// txtTitle
			// 
			this->txtTitle->AutoSize = true;
			this->txtTitle->Font = (gcnew System::Drawing::Font(L"Sitka Small", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txtTitle->Location = System::Drawing::Point(37, 90);
			this->txtTitle->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->txtTitle->Name = L"txtTitle";
			this->txtTitle->Size = System::Drawing::Size(852, 116);
			this->txtTitle->TabIndex = 0;
			this->txtTitle->Text = L"Algoritmo Dijkstra";
			this->txtTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// btnManual
			// 
			this->btnManual->BackColor = System::Drawing::SystemColors::Window;
			this->btnManual->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnManual->Location = System::Drawing::Point(173, 267);
			this->btnManual->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->btnManual->Name = L"btnManual";
			this->btnManual->Size = System::Drawing::Size(267, 49);
			this->btnManual->TabIndex = 1;
			this->btnManual->Text = L"Modo Manual";
			this->btnManual->UseVisualStyleBackColor = false;
			this->btnManual->Click += gcnew System::EventHandler(this, &mainForm::btnManual_Click);
			// 
			// btnAutomatico
			// 
			this->btnAutomatico->BackColor = System::Drawing::SystemColors::Window;
			this->btnAutomatico->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnAutomatico->Location = System::Drawing::Point(509, 267);
			this->btnAutomatico->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->btnAutomatico->Name = L"btnAutomatico";
			this->btnAutomatico->Size = System::Drawing::Size(267, 49);
			this->btnAutomatico->TabIndex = 2;
			this->btnAutomatico->Text = L"Modo Automatico";
			this->btnAutomatico->UseVisualStyleBackColor = false;
			this->btnAutomatico->Click += gcnew System::EventHandler(this, &mainForm::btnAutomatico_Click);
			// 
			// btnComoFunciona
			// 
			this->btnComoFunciona->BackColor = System::Drawing::SystemColors::Window;
			this->btnComoFunciona->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnComoFunciona->Location = System::Drawing::Point(173, 372);
			this->btnComoFunciona->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->btnComoFunciona->Name = L"btnComoFunciona";
			this->btnComoFunciona->Size = System::Drawing::Size(267, 49);
			this->btnComoFunciona->TabIndex = 3;
			this->btnComoFunciona->Text = L"Como Funciona";
			this->btnComoFunciona->UseVisualStyleBackColor = false;
			this->btnComoFunciona->Click += gcnew System::EventHandler(this, &mainForm::btnComoFunciona_Click);
			// 
			// btnIntegrantes
			// 
			this->btnIntegrantes->BackColor = System::Drawing::SystemColors::Window;
			this->btnIntegrantes->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnIntegrantes->Location = System::Drawing::Point(509, 372);
			this->btnIntegrantes->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->btnIntegrantes->Name = L"btnIntegrantes";
			this->btnIntegrantes->Size = System::Drawing::Size(267, 49);
			this->btnIntegrantes->TabIndex = 4;
			this->btnIntegrantes->Text = L"Integrantes";
			this->btnIntegrantes->UseVisualStyleBackColor = false;
			this->btnIntegrantes->Click += gcnew System::EventHandler(this, &mainForm::btnIntegrantes_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::SystemColors::Control;
			this->groupBox1->Controls->Add(this->btnManual);
			this->groupBox1->Controls->Add(this->txtTitle);
			this->groupBox1->Controls->Add(this->btnIntegrantes);
			this->groupBox1->Controls->Add(this->btnAutomatico);
			this->groupBox1->Controls->Add(this->btnComoFunciona);
			this->groupBox1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->groupBox1->Location = System::Drawing::Point(33, 36);
			this->groupBox1->Margin = System::Windows::Forms::Padding(7, 6, 7, 6);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(7, 6, 7, 6);
			this->groupBox1->Size = System::Drawing::Size(976, 495);
			this->groupBox1->TabIndex = 5;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L" ";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &mainForm::groupBox1_Enter);
			// 
			// mainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->ClientSize = System::Drawing::Size(1040, 562);
			this->Controls->Add(this->groupBox1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->Name = L"mainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"mainForm";
			this->Load += gcnew System::EventHandler(this, &mainForm::mainForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}

#pragma endregion
	private: System::Void mainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void btnAutomatico_Click(System::Object^ sender, System::EventArgs^ e) {
		Representacion_Dijkstra^ f = gcnew Representacion_Dijkstra();
		f->SetMode(false);
		f->ShowDialog();
	}
	private: System::Void btnManual_Click(System::Object^ sender, System::EventArgs^ e) {
		Representacion_Dijkstra^ f = gcnew Representacion_Dijkstra();
		f->SetMode(true);
		f->ShowDialog();
	}
	private: System::Void btnComoFunciona_Click(System::Object^ sender, System::EventArgs^ e) {
		Como_funciona^ f = gcnew Como_funciona();
		f->ShowDialog();
	}
	private: System::Void btnIntegrantes_Click(System::Object^ sender, System::EventArgs^ e) {

		MessageBox::Show(" -> Mauricio Alonso Calderon Zavalaga\n -> Lisander Reynaldo Concha Urquizo\n -> Khaled Yair Loarte Macedo\n -> Jaise Joel Prieto Reyes\n -> Cielo Mariana Vargas Velarde", "Grupo 03 - Mate Computacional");
	}
	private: System::Void groupBox1_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
