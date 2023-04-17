#include "NorthWestAngle.h"

namespace LinearEquations {
//-----------------------------------------------------------------------------------
NorthWestAngle::NorthWestAngle() {

}
//-----------------------------------------------------------------------------------
void NorthWestAngle::Process() {
	PrintMatrix(m_table);
	int stroke = 0;
	int column = 0;

	while (stroke < m_vendorCount && column < m_consumerCount) {
		Fraction::SimpleFraction stocksCount = m_table[stroke].back().Cost;
		Fraction::SimpleFraction needsCount = m_table.back()[column].Cost;

		Fraction::SimpleFraction volume = stocksCount < needsCount ? stocksCount : needsCount;

		m_table[stroke][column].Volume = volume;
		m_table[stroke].back().Cost -= volume;
		m_table.back()[column].Cost -= volume;

		if (stocksCount < needsCount) {
			m_table[stroke][column].Filled = true;
			ZeroindStroke(stroke, column);
			stroke++;
		}
		else if(stocksCount > needsCount || stroke == m_vendorCount - 1) {
			m_table[stroke][column].Filled = true;
			ZeroingColumn(stroke, column);
			column++;
		}
		else {
			std::cout << "The reference plan is degenerate." << std::endl;

			m_table[stroke][column].Filled = true;
			ZeroindStroke(stroke, column);
			ZeroingColumn(stroke, column);

			if (stroke < m_vendorCount - 1) {
				m_table[stroke + 1][column].Volume = Fraction::SimpleFraction();
				m_table[stroke + 1][column].Unused = false;
			}
			else if (column < m_consumerCount - 1) {
				m_table[stroke][column + 1].Volume = Fraction::SimpleFraction();
				m_table[stroke][column + 1].Unused = false;
			}

			stroke++;
			column++;
		}

		PrintMatrix(m_table);
	}
	Finishing();
}
//-----------------------------------------------------------------------------------
void NorthWestAngle::Finishing() {
	int filledCells = 0;
	Fraction::SimpleFraction totalCost;

	for (int i = 0; i < m_vendorCount; i++) {
		for (int j = 0; j < m_consumerCount; j++) {
			if (!m_table[i][j].Unused) {
				totalCost += m_table[i][j].Cost * m_table[i][j].Volume;
				filledCells++;
			}
		}
	}

	std::cout << "Filled cells: " << filledCells << ", need: " << m_consumerCount + m_vendorCount - 1 << std::endl;
	std::cout << "Total cost: " << totalCost << std::endl;
}
//-----------------------------------------------------------------------------------
TransportTable NorthWestAngle::ReadMatrixFromFile(std::string fileName) {
	std::fstream file(fileName);

	if (!file.is_open()) {
		std::cout << "Error opening the file" << std::endl;
	}

	std::string line = "";

	while (getline(file, line)) {
		m_table.push_back(std::vector<Cell>());
		while (line.size() != 0) {
			int spaceInd = line.find(" ");

			Cell cell;
			cell.Cost = Fraction::SimpleFraction(std::stoi(line.substr(0, spaceInd)), 1);
			m_table.back().push_back(cell);

			line.erase(0, spaceInd + 1);
			if (spaceInd == -1)
				break;
		}
	}

	m_vendorCount = m_table.size() - 1;
	m_consumerCount = m_table[0].size() - 1;

	for (int i = 0; i < m_vendorCount; i++) {
		m_stocksCount += m_table[i].back().Cost;
	}

	for (auto& need : m_table.back()) {
		m_needsCount += need.Cost;
	}

	PrintMatrix(m_table);

	std::cout << "Stocks count = " << m_stocksCount << ", Needs count = " << m_needsCount
			  << " => Transport model is " << (m_stocksCount == m_needsCount ? "closed." : "opened.") << std::endl << std::endl;

	if (m_stocksCount < m_needsCount) {
		std::vector<Cell> newVendor(m_consumerCount, Cell());
		newVendor.push_back(Cell{ m_needsCount - m_stocksCount });

		m_table.insert(m_table.cbegin() + m_vendorCount, newVendor);
	}
	else if (m_stocksCount > m_needsCount) {
		for (int i = 0; i < m_vendorCount; i++) {
			m_table[i].insert(m_table[i].cbegin() + m_consumerCount, Cell());
		}
		m_table.back().push_back(Cell{ m_stocksCount - m_needsCount });
	}

	return m_table;
}
//-----------------------------------------------------------------------------------
void NorthWestAngle::PrintMatrix(TransportTable& table) {
	for (const auto& line : table) {
		for (const auto& column : line) {
			std::cout << column << "\t\t";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}
//-----------------------------------------------------------------------------------
void NorthWestAngle::ZeroingColumn(int strokeNum, int columnNum) {
	for (int i = 0; i < m_table.size() - 1; i++) {
		if (m_table[i][columnNum].Filled) continue;
		m_table[i][columnNum].Unused = true;
	}
}
//-----------------------------------------------------------------------------------
void NorthWestAngle::ZeroindStroke(int strokeNum, int columnNum) {
	for (int j = 0; j < m_table[strokeNum].size() - 1; j++) {
		if (m_table[strokeNum][j].Filled) continue;
		m_table[strokeNum][j].Unused = true;
	}
}
//-----------------------------------------------------------------------------------
}