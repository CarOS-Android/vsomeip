import os

os.system(f'docker run --rm -it -v {os.getcwd()}:/app --net=host vsomeip-builder bash')

