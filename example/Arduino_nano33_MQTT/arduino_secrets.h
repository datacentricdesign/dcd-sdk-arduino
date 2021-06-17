// DCD Hub details
const char *project_id = "Arduino Nano 33 IoT";
const char *thing_id = "dcd:things:s0m3-numb3rs-4nd-d1g17s";

// Configuration for JWT Token
const char *private_key_str =
  "03:2b:ae:b0:44:28:11:46:5a:10:cc:b2:22:f1:f4:"
  "20:92:cb:02:ec:88:54:a3:7b:3d:58:a7:47:4a:27:"
  "26:f5";

 /*
  * How to get the private key:
  * ===========================
  *
  * First, generate a public/private key pair.
  * On Mac OS, you can directly open a terminal.
  * On Windows, you first need to install OpenSSL:
  * https://slproweb.com/products/Win32OpenSSL.html
  *
  * Enter these commands in a terminal to generate your keys:
  * openssl ecparam -genkey -name prime256v1 -noout -out ec_private.pem
  * openssl ec -in ec_private.pem -pubout -out ec_public.pem
  *
  * The public key is needed on the DCD Hub to create a new thing.
  * The corresponding private key is needed here.
  * Use the following command to convert your key to hex digits:
  * openssl ec -in ec_private.pem -noout -text
  *
  * Copy the priv: part.
  * The key should be exactly 32 pairs of hex digits long.
  * If it's longer and starts with "00:" delete the "00:". If
  * it's shorter add "00:" to the beginning.
  */
