set -euo pipefail
ACL_VERSION=1.5.1

if [ ! -e "ac-library-${ACL_VERSION}.tar.gz" ]; then
  curl -LJO "https://github.com/atcoder/ac-library/archive/refs/tags/v${ACL_VERSION}.tar.gz"
fi

if [ ! -e "ac-library" ]; then
  tar xf "ac-library-${ACL_VERSION}.tar.gz"
  mv "ac-library-${ACL_VERSION}" ac-library
fi
