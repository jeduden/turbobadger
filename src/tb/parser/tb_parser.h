// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================

#ifndef TB_PARSER_H
#define TB_PARSER_H

#include "tb_value.h"
#include "tb_tempbuffer.h"
#include "tb_str.h"

namespace tb {

class TBParserTarget
{
public:
	virtual ~TBParserTarget() {}
	virtual void OnError(int line_nr, const char *error) = 0;
	virtual void OnComment(int line_nr, const char *comment) = 0;
	virtual void OnToken(int line_nr, const char *name, TBValue &value) = 0;
	virtual void Enter() = 0;
	virtual void Leave() = 0;
};

class TBParserStream
{
public:
	virtual ~TBParserStream() {}
	virtual int GetMoreData(char *buf, int buf_len) = 0;
};

class TBParser
{
public:
	enum STATUS {
		STATUS_OK,
		STATUS_NO_MEMORY,
		STATUS_PARSE_ERROR
	};
	TBParser() {}
	STATUS Read(TBParserStream *stream, TBParserTarget *target);
private:
	int current_indent;
	int current_line_nr;
	TBStr multi_line_token;
	TBTempBuffer multi_line_value;
	int multi_line_sub_level;
	bool pending_multiline;
	void OnLine(char *line, TBParserTarget *target);
	void OnCompactLine(char *line, TBParserTarget *target);
	void OnMultiline(char *line, TBParserTarget *target);
	void ConsumeValue(TBValue &dst_value, char *&line);
};

}; // namespace tb

#endif // TB_PARSER_H