#pragma once

#include <ehs/Types.h>
#include <ehs/Str.h>

class Section : public ehs::Vector<ehs::Byte>
{
private:
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::UInt_64 offset;
	ehs::UInt_8 alignment;

public:
	Section();

	Section(ehs::Str_8 name, const ehs::UInt_8 &alignment);

	Section(Section &&other) noexcept;

	Section(const Section &other);

	Section &operator=(Section &&other) noexcept;

	Section &operator=(const Section &other);

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	void SetOffset(const ehs::UInt_64 &newOffset);

	ehs::UInt_64 GetOffset() const;

	ehs::UInt_8 GetAlignment() const;

};
