import numpy as np

# Recupera os dados do arquivo .txt
def read_config_txt(filename):
    data = {}
    
    with open(filename, 'r') as file:
        data['h'] = float(file.readline().strip())
        data['y_fix'] = float(file.readline().strip())
        
        data['muscle_thick'], data['fat_thick'], data['dermis_thick'] = map(
            float, file.readline().split()
        )
        
        coords = list(map(float, file.readline().split()))
        data['xt1'], data['yt1'], data['zt1'], data['rt1'], data['xt2'], data['yt2'], data['zt2'], data['rt2'] = coords
        
        data['x0'], data['xf'], data['z0'], data['zf'] = map(
            float, file.readline().split()
        )
        
        data['T_a'] = float(file.readline().strip())
        data['T_thresh1'], data['T_thresh2'] = map(
            float, file.readline().split()
        )
        
        data['tol'] = float(file.readline().strip())
        data['tf'] = float(file.readline().strip())
        data['snapshot_interval'] = int(file.readline().strip())

    
    return data

# Recupera os dados da matriz 
def read_matrix(filename, tamz, tamx, type):
    
    with open(filename, 'rb') as f:
        matrix = np.fromfile(f, dtype=type).reshape((tamz, tamx))
        
    return matrix
