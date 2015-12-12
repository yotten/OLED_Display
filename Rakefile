CC = "gcc"
CXX = "g++"
CFLAGS="-Wall -fPIC -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s" 
CPPFLAGS="-Wall -fPIC -fno-rtti -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s"

# Where you want it installed when you do 'make install'
PREFIX="/usr/local"

# Library parameters
# where to put the lib
LIBDIR="#{PREFIX}/lib"
# lib name
LIB="libssd1306"
# shared library name
LIBNAME="#{LIB}.so.1.0"

task :default => ["libssd1306", "install", "install_header"]

task :all => ["clean", :default]

task :install do
  puts "\n[Install Library]"
  sh "sudo install -m 0755 #{LIBNAME} #{LIBDIR}"
  sh "sudo ln -sf #{LIBDIR}/#{LIBNAME} #{LIBDIR}/#{LIB}.so.1"
  sh "sudo ln -sf #{LIBDIR}/#{LIBNAME} #{LIBDIR}/#{LIB}.so"
  sh "sudo ldconfig"
  #rm -rf ${LIB}.*
end

task :install_header do
  puts "\n[Install Headers]"
  sh "sudo cp -f  Adafruit_*.h #{PREFIX}/include"
  sh "sudo cp -f  ArduiPi_*.h #{PREFIX}/include"
  sh "sudo cp -f  bcm2835.h #{PREFIX}/include"
end

file "libssd1306" => ["Adafruit_SSD1306.o", "Adafruit_GFX.o", "bcm2835.o"] do
  puts "\n[libssd1306]"
  sh "#{CXX} -shared -Wl,-soname,libssd1306.so.1 -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s  -o libssd1306.so.1.0 Adafruit_SSD1306.o Adafruit_GFX.o bcm2835.o"
end

file "Adafruit_SSD1306.o" => "Adafruit_SSD1306.cpp" do
  puts "\n[Adafruit_SSD1306.o]"
  sh "#{CXX} #{CPPFLAGS} -c Adafruit_SSD1306.cpp"
end

file "Adafruit_GFX.o" => "Adafruit_GFX.cpp" do
  puts "\n[Adafruit_GFX.o]"
  sh "#{CXX} #{CPPFLAGS} -c Adafruit_GFX.cpp" 
end

file "bcm2835.o" => "bcm2835.c" do
  puts "\n[bcm2835.o]"
  sh "#{CC} #{CFLAGS} -c bcm2835.c"
end 

task "clean" do
  puts "\n[clean]"
  sh "sudo rm -rf *.o libssd1306.* /usr/local/lib/libssd1306.*"
end

task "demo" do
  sh "sudo ./examples/ssd1306_demo"
end
