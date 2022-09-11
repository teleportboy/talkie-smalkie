#ifndef JSON_PARSER_INCLUDED_H
#define JSON_PARSER_INCLUDED_H

#include <json-c/json.h>

char* json_get_property(json_object* json, char* property);
void json_set_array_property(json_object** array, char* value);
void json_set_value(json_object* json, char* key, char* value);

#endif