for i in {1..7}
do
  let k=${i}*10
  head -c ${k}K /dev/urandom | tr -dc A-Za-z0-9 | head -c ${i}K  >tmp/${i}
done