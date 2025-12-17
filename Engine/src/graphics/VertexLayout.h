#pragma once
#include <vector>

namespace eng
{
	struct VertexElement
	{
		unsigned int index; // Attribute location
		unsigned int size;  // Number of Components
		unsigned int type;  // Datatype (e.g. GL_FLOAT)
		uint32_t offset;    // Bytes offset from start of vertex

		static constexpr int PositionIndex = 0;
		static constexpr int ColorIndex = 1;
		static constexpr int UVIndex = 2;
		static constexpr int NormalIndex = 3;
	};

	struct VertexLayout
	{
		std::vector<VertexElement> elements;
		uint32_t stride = 0; // Total size of a single vertex
	};
}