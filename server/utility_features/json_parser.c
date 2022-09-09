#include "json_parser.h"

char* json_get_property(json_object* json, char* property) {
    json_object* property_obj = json_object_object_get(json, property);
    return (char*)json_object_get_string(property_obj);
}

void json_set_array_property(json_object** array, char* value) {
    json_object* item = json_object_new_string(value);
    json_object_array_add(*array, item);
}

void json_set_value(json_object* json, char* key, char* value) {
    json_object* new_data = json_object_new_string(value);
    json_object_object_add(json, key, new_data);
}