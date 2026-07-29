#include "arctyx/compiler/Section.h"

Section::Section()
	: id(0), offset(0), alignment(0)
{
}

Section::Section(ehs::Str_8 name, const ehs::UInt_8 &alignment)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), offset(0), alignment(alignment)
{
}

Section::Section(Section&& other) noexcept
	: Vector((Vector &&)other), id(other.id), name((ehs::Str_8 &&)other.name), offset(other.offset),
	alignment(other.alignment)
{
	other.id = 0;
	other.offset = 0;
	other.alignment = 0;
}

Section::Section(const Section& other)
	: Vector(other), id(other.id), name(other.name), offset(other.offset), alignment(other.alignment)
{
}

Section& Section::operator=(Section&& other) noexcept
{
	if (this == &other)
		return *this;

	Vector::operator=((Vector &&)other);

	id = other.id;
	name = (ehs::Str_8 &&)other.name;
	offset = other.offset;
	alignment = other.alignment;

	other.id = 0;
	other.offset = 0;
	other.alignment = 0;

	return *this;
}

Section& Section::operator=(const Section& other)
{
	if (this == &other)
		return *this;

	Vector::operator=(other);

	id = other.id;
	name = other.name;
	offset = other.offset;
	alignment = other.alignment;

	return *this;
}

ehs::UInt_64 Section::GetId() const
{
	return id;
}

ehs::Str_8 Section::GetName() const
{
	return name;
}

void Section::SetOffset(const ehs::UInt_64& newOffset)
{
	offset = newOffset;
}

ehs::UInt_64 Section::GetOffset() const
{
	return offset;
}

ehs::UInt_8 Section::GetAlignment() const
{
	return alignment;
}
