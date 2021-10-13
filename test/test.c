
int main(void) {
  int x, y, z, *p;
  p = &x;
  x = 100;
  y = 200;
  if (p) {
    y = 300;
    z = 400;
  } else {
    y = 500;
    z = 600;
  }
  *p = 700;
  return x;
}
