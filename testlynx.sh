if ! [ -x "$(command -v lynx)" ]; then
  echo 'Error: lynx is not installed.' >&2
  exit 1
fi