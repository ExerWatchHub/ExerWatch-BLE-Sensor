class ExerProgram {
protected:
  bool is_init = false;
  String name = "NO_NAME";

public:
  virtual void init() {};
  virtual void update() {};
  virtual void cleanup() {};
  virtual const char* getName() {
    return name.c_str();
  };
};