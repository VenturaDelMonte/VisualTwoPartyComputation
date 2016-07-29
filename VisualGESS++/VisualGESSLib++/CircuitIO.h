#pragma once

struct VGESS_API CircuitIO
{
	CCircuit* circuit;
	u32 inputs;

};

class CRasterImage;
class IGate;

struct VGESS_API CircuitImageNode
{
	CRasterImage* img;
	IGate* gate;
	u32 x, y;
};

typedef void (*OnDrawingFunction)(CircuitImageNode*);
extern OnDrawingFunction drawing_callback;

typedef void (*OnStartDrawingFunction)(u32, u32);
extern OnStartDrawingFunction start_drawing_callback;
