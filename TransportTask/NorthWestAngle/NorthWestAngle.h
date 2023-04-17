//-------------------------------------------------------------------------------------------
// NorthWestAngle.h
// Description: Класс, реализующий метод северо-западного угла
// Date: 17.04.2023
// Authors: Хьюго М.А. & Наумов Н.В.
// Ⓒ Sibsutis university
//-------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>

#include "../SimpleFraction/SimpleFraction.h"

struct Cell {
	Fraction::SimpleFraction Cost;
	Fraction::SimpleFraction Volume;

	bool Filled = false;
	bool Unused = false;
	bool ConsumerOrVendor = false;
};

inline std::ostream& operator<< (std::ostream& out, const Cell& cell) {
	if (cell.Unused) {
		out <<  " - | - ";
	}
	else if (cell.ConsumerOrVendor) {
		out << cell.Cost;
	}
	else {
		out << cell.Volume << " | " << cell.Cost;
	}
	return out;
}

typedef std::vector<std::vector<Cell>> TransportTable;

namespace LinearEquations {
class NorthWestAngle {
public:
	NorthWestAngle();

	void Process();
	void Finishing();

	TransportTable ReadMatrixFromFile(std::string fileName = "NorthWestAngle\\Matrix.txt");
	void PrintMatrix(TransportTable& matrix);

protected:
	int m_vendorCount = 0;
	int m_consumerCount = 0;

	Fraction::SimpleFraction m_needsCount;
	Fraction::SimpleFraction m_stocksCount;

	TransportTable m_table;

	void ZeroingColumn(int strokeNum, int columnNum);
	void ZeroindStroke(int strokeNum, int columnNum);
};
}