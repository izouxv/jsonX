/*
 ============================================================================
 Name		: String.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString declaration
 ============================================================================
 */

#ifndef STRING_H
#define STRING_H

// INCLUDES 

#include "Object.h"
class Integer;
class TRgb;
class List;
class TPtr;
class TPtr8;
class String: public Object {
public:
//	String(const String& str);

	String(const Object& object);

	String(const TPtr& des);

	String(const TPtr8& des);

	String(const char* chars);
	
	void SET(const String& aString);
	
	const TPtr& desc() const;
	
	TPtr des();

	operator const TPtr&() const;

	String& operator=(const TPtr& thing);

	String& operator=(const TPtr8& thing);

	String& operator=(const char* thing);
    
	String operator+(const Integer& integer) const;
    
	String operator+(const String& string) const;

	String operator+(const TPtr& string) const;

	String operator+(const char* string) const;
    
//	void operator+=(const Integer& integer);
    
	void operator+=(const String& string);

	void operator+=(const TPtr& string);

	void operator+=(const char* string);

	bool startsWith(const String& str) const;

	bool startsWithIgnoreCase(const String& str) const;

	bool endsWith(const String& str) const;

	bool endsWithIgnoreCase(const String& str) const;

	TInt16 charAt(int index) const;

	String toLowerCase() const;

	String toUpperCase() const;

	String substring(int start, int end = -1) const;

	int length() const;

	//Bytes getBytes() const;
    
	int crc32() const;
	
	String md5() const;//32bytes visible char

	Bytes md5Bytes() const;
    
    const char* getCharc()const;

	Object toObject() const;

	void remove(const TPtr& aWord);

	//by zouxu/////////////////////////////// 
	//	TInt64 toInt();
	//static String Mem();
	List Split(char aSeperate)  ;
	static bool isArabic(TUint16 ch);
	static TBool IsCJK(TUint16 ch);
	static TBool IsMarks(TUint16 ch);
	enum TFuckingType {
		E_SQUARE = 0, E_L2R, E_R2L, E_Marks, E_NewLine, E_Smile
	};
	//static void fuck(TFuckingType type, const TPtr& aString, List aList, 	const List& aSmileList, TBool havaFaceTag = ETrue);
	//0:SQUARE,1:L2R,2:R2L,3:Marks,4:NewLine,5:Smile
	List lines(const List& aSmileList) const;
	TRgb toRgb();
	//	virtual String toString();
	///END//////////////////////////////////////

private:
    void initWithChar(const char* chars);

#ifdef __WINS__
	void updateHandle(Handle* value);
	StringHandle* internal;
#endif

};

#endif // STRING_H
