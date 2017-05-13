put in directory:
 * run-centrality-for-fb.py
 * centrality-omp-dijkstra.cpp
 * Makefile

in terminal, run:
bash
export PYTHONPATH=/afs/cs/academic/class/15418-s17/public/sw/python/:$PYTHONPATH
export PATH=/afs/cs/academic/class/15418-s17/public/sw/python/:$PATH
pip install --user -e git+https://github.com/mobolic/facebook-sdk.git#egg=facebook-sdk
make

go to https://developers.facebook.com/tools/explorer/
get token with user_friends permission
copy the token to beginning of run-centrality-for-fb.py

in terminal, run:
python run-centrality-for-fb.py
