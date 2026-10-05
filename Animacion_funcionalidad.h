#pragma once
using namespace System;
using namespace System::Drawing;

ref class Fondo_Instrucciones
{
private:
	int idX, idY;
	int ancho, alto;
public:
	Fondo_Instrucciones() {
		idX = 0;
		idY = 0;
		ancho = 0;
		alto = 0;
	};
	~Fondo_Instrucciones() {};
	void dibujar(Graphics^ g, Bitmap^ bmpFondo_instrucciones);
	void mover(Graphics^ g);

};
inline void Fondo_Instrucciones::dibujar(Graphics^ g, Bitmap^ bmpFondo_instrucciones)
{
	ancho = bmpFondo_instrucciones->Width / 5;
	alto = bmpFondo_instrucciones->Height / 10;
	System::Drawing::Rectangle sectionShow = System::Drawing::Rectangle(idX * ancho, idY * alto, ancho, alto);
	System::Drawing::Rectangle destino = System::Drawing::Rectangle(0, 0, g->VisibleClipBounds.Width, g->VisibleClipBounds.Height);
	g->DrawImage(bmpFondo_instrucciones, destino, sectionShow, GraphicsUnit::Pixel);
}
inline void Fondo_Instrucciones::mover(Graphics^ g)
{
	idX++;
	if (idX >= 5)
	{
		idX = 0;
		idY++;
		if (idY >= 10)
		{
			idY = 0;
		}
	}
}