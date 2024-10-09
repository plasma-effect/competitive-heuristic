set -euo pipefail
BOOST_MINOR=82
BOOST_PATCH=0
SHA256="66a469b6e608a51f8347236f4912e27dc5c60c60d7d53ae9bfe4683316c6f04c"

if [ ! -e "boost_1_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz" ]; then
  curl -O "https://archives.boost.io/release/1.${BOOST_MINOR}.${BOOST_PATCH}/source/boost_1_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz"
fi
echo "${SHA256} boost_1_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz" | sha256sum -c -

if [ ! -e "boost_1_${BOOST_MINOR}_${BOOST_PATCH}" ]; then
  tar xf "boost_1_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz"
fi

if [ ! -e "boost" ]; then
  mkdir -p boost
  cd "boost_1_${BOOST_MINOR}_${BOOST_PATCH}"
  ./bootstrap.sh
  ./b2 install -j4 --prefix=$PWD/../boost
fi
