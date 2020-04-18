export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"
. ../../setCompilePath.sh
if [ ! -d libressl/.git ]
then
  git clone https://github.com/libressl-portable/portable.git
  git checkout OPENBSD_6_6
  sed -i 's/program_invocation_short_name/"?"/g' portable/crypto/compat/getprogname_linux.c
fi

cd portable/
./autogen.sh
./configure --prefix=${INSTALL} --host=mips-linux-gnu --with-pic
make -j4
make install

cp -r */.libs/*.a ${INSTALL}/lib
cp -rL */.libs/*.la ${INSTALL}/lib
cp -rL */.libs/*.so ${INSTALL}/lib
cp --preserve=links */.libs/*.so* ${INSTALL}/lib

cp -r include ${INSTALL}/
# Compile openssl binary by hand, didn't find how to use static lib instead of dynamic
export PREF=${INSTALL}
cd apps/openssl
${CC} -std=gnu99 ${CFLAGS} -Wall -std=gnu99 -fno-strict-aliasing -fno-strict-overflow -D_FORTIFY_SOURCE=2 -fstack-protector-all -DHAVE_GNU_STACK -Wno-pointer-sign -Wl,-z -Wl,relro -Wl,-z -Wl,now -o openssl.nolib cms.o apps.o asn1pars.o ca.o ciphers.o crl.o crl2p7.o dgst.o dh.o dhparam.o dsa.o dsaparam.o ec.o ecparam.o enc.o errstr.o gendh.o gendsa.o genpkey.o genrsa.o nseq.o ocsp.o openssl.o passwd.o pkcs12.o pkcs7.o pkcs8.o pkey.o pkeyparam.o pkeyutl.o prime.o rand.o req.o rsa.o rsautl.o s_cb.o s_client.o s_server.o s_socket.o s_time.o sess_id.o smime.o speed.o spkac.o ts.o verify.o version.o x509.o certhash.o apps_posix.o compat/strtonum.o ../../ssl/tls13_error.o ${PREF}/lib/libssl.a ${PREF}/lib/libcrypto.a -lpthread -Wl,-rpath -Wl,${PREF}/_install/lib
cp openssl.nolib ${PREF}/bin/openssl
