class OBC1 : public Memory {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read(uint64_t addr);
  void write(uint64_t addr, uint8 data);

  void serialize(serializer&);
  OBC1();
  ~OBC1();

private:
  uint8 ram_read(uint64_t addr);
  void ram_write(uint64_t addr, uint8 data);

  struct {
    uint16 address;
    uint16 baseptr;
    uint16 shift;
  } status;
};

extern OBC1 obc1;
