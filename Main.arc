Namespace ehs
{
	Class Bar
	{
		Bool enabled

		Bar(const Bool enabled)
		{
			this->enabled = enabled
		}
	}

	Bool IsTrue()
	{
		Return True;
	}

	Unt_8 Main(const Char_8 *args)
	{
		Char_8 hello[] = "Hello, fucking world!"

		Bar bar(true)
		bar.enabled = False

		UInt_8 test = 5, a = 2
		test = test + 25
		test = test + a
		++test
		a++

		if (test < a)
			++test

		Return test
	}

	Bool a = True
	Bool b = False
}