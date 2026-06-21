#include "httplib.h"
#include <iostream>
#include <cmath>

using namespace std;

float Addition(float a, float b);
float Substraction(float a, float b);
float Multiplication(float a, float b);
float Division(float a, float b, bool &ok);
float Modulo(float a, float b, bool &ok);
float Sin(float degress);
float Cos(float degress);
float Cot(float degrees, bool &ok);
float Log(float a, bool &ok);
float Square(float a);
float Cube(float a);

string jsonResult(float value);
string jsonError(const string &message);

int main() {
  httplib::Server svr;

  svr.set_default_headers({
    {"Access-Control-Allow-Origin", "*"}
  });

  auto getParam = [](const httplib::Request &req, const string &name, float fallback){
    if (req.has_param(name)){
      return stof(req.get_param_value(name));
    }
    return fallback;
  };
  
  svr.Get("/add", [&](const httplib::Request &req, httplib::Response &res){
    float a = getParam(req, "a", 0);
    float b = getParam(req, "b", 0);

    res.set_content(jsonResult(Addition(a,b)), "application/json");
  });

  svr.Get("/subtract", [&](const httplib::Request &req, httplib::Response &res) {
    float a = getParam(req, "a", 0);
    float b = getParam(req, "b", 0);
    res.set_content(jsonResult(Substraction(a, b)), "application/json");
  });

  svr.Get("/multiply", [&](const httplib::Request &req, httplib::Response &res) {
    float a = getParam(req, "a", 0);
    float b = getParam(req, "b", 0);
    res.set_content(jsonResult(Multiplication(a, b)), "application/json");
  });

  svr.Get("/divide", [&](const httplib::Request &req, httplib::Response &res) {
    float a = getParam(req, "a", 0);
    float b = getParam(req, "b", 0);
    bool ok = true;
    float r = Division(a, b, ok);
    if (!ok) { 
      res.set_content(jsonError("Divide can't with 0'"), "application/json"); 
      return; 
    }
    res.set_content(jsonResult(r), "application/json");
  });

  svr.Get("/modulo", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      float b = getParam(req, "b", 0);
      bool ok = true;
      float r = Modulo(a, b, ok);
      if (!ok) { 
        res.set_content(jsonError("Module can't with 0'"), "application/json"); 
        return; 
      }
      res.set_content(jsonResult(r), "application/json");
  });

  svr.Get("/sin", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      res.set_content(jsonResult(Sin(a)), "application/json");
  });

  svr.Get("/cos", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      res.set_content(jsonResult(Cos(a)), "application/json");
  });

  svr.Get("/cot", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      bool ok = true;
      float r = Cot(a, ok);
      if (!ok) { 
        res.set_content(jsonError("cot out of rule"), "application/json"); 
        return; 
      }
      res.set_content(jsonResult(r), "application/json");
  });

  svr.Get("/log", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      bool ok = true;
      float r = Log(a, ok);
      if (!ok) { 
        res.set_content(jsonError("Value must > 0"), "application/json"); 
        return; 
      }
      res.set_content(jsonResult(r), "application/json");
  });

  svr.Get("/square", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      res.set_content(jsonResult(Square(a)), "application/json");
  });

  svr.Get("/cube", [&](const httplib::Request &req, httplib::Response &res) {
      float a = getParam(req, "a", 0);
      res.set_content(jsonResult(Cube(a)), "application/json");
  });

  cout << "KISM server running at http://localhost:8080" << std::endl;

  svr.listen("0.0.0.0", 8080);

  return 0;
}

float Addition(float a, float b) { return a + b; }

float Substraction(float a, float b) { return a - b; }

float Multiplication(float a, float b) { return a * b; }

float Division(float a, float b, bool &ok){
  if (b == 0.0) {
    ok = false;
    return 0.0;
  }

  return a/b;
}

float Modulo(float a, float b, bool &ok){
  if (b == 0.0) {
    ok = false;
    return 0.0;
  }
    
  return fmod(a, b);
}

float Sin(float degrees){
  const float PI = acos(-1.0);
  return sin(degrees * PI / 180.0);
}

float Cos(float degrees){
    const float PI = acos(-1.0);
    return cos(degrees * PI / 180.0);
}

float Cot(float degrees, bool &ok){
    const float PI = acos(-1.0);
    float t = tan(degrees * PI / 180.0);
    if (t == 0){
      ok = true;
      return 0.0;
    }

    return 1.0/t;
}

float Log(float a, bool &ok){
  if ( a <= 0.0) {
    ok = false;
    return 0.0;
  }
  
  return log10(a);
}

float Square(float a) { return a * a; }
float Cube(float a) { return a * a * a; }

string jsonResult(float value){
  ostringstream out;

  out << "{\"ok\": true, \n\"result\":" << value << "}";
  return out.str();
}

string jsonError(const string &message){
  ostringstream out;
  
  out << "{\"ok\": false, \n\"result\":" << message << "}";
  return out.str();
}
