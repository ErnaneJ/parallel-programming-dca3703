"""
visualize.py

Visualizes 3D snapshots of a simulated fluid using Plotly.
"""

import numpy as np
import plotly.graph_objects as go
import os

# Configuration
N = 32
SNAPSHOT_INTERVAL = 10
NSTEPS = 1000
filename_with_perturb = './task-11.impact-of-schedule-and-collapse-clauses/data/fluid_with_perturbation.bin'

def read_snapshots(filename):
    """
    Reads binary snapshots of 3D float32 arrays from a file.

    Parameters:
        filename (str): Path to the binary file.

    Returns:
        tuple: A tuple (data, n_snapshots) where:
            - data is a 4D numpy array [n_snapshots, N, N, N]
            - n_snapshots is the number of snapshots found in the file
    """
    size_in_bytes = os.path.getsize(filename)
    snapshot_size = N * N * N * 4  # float32 = 4 bytes
    n_snapshots = size_in_bytes // snapshot_size
    print(f"Detected {n_snapshots} snapshots in file '{filename}'.")

    with open(filename, 'rb') as f:
        data = np.fromfile(f, dtype=np.float32)
    data = data.reshape((n_snapshots, N, N, N))
    return data, n_snapshots

def plot_with_slider(snapshots, n_snapshots):
    """
    Plots a slider-enabled 3D surface visualization of snapshot slices.

    Parameters:
        snapshots (numpy.ndarray): Array of shape (n_snapshots, N, N, N)
        n_snapshots (int): Total number of snapshots
    """
    mid = N // 2  # Take middle Z-slice

    frames = []
    for idx, snapshot in enumerate(snapshots):
        slice_2d = snapshot[:, :, mid]
        frame = go.Frame(
            data=[go.Surface(z=slice_2d, colorscale="Viridis")],
            name=str(idx)
        )
        frames.append(frame)

    # Initial frame
    slice_2d = snapshots[0][:, :, mid]
    fig = go.Figure(
        data=[go.Surface(z=slice_2d, colorscale="Viridis")],
        frames=frames
    )

    fig.update_layout(
        title="Fluid Perturbation Evolution",
        scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='Velocity',
            camera_eye=dict(x=1.5, y=1.5, z=0.5)
        ),
        width=800,
        height=700,
        updatemenus=[dict(
            type="buttons",
            buttons=[dict(label="Play", method="animate", args=[None, {"frame": {"duration": 100, "redraw": True}, "fromcurrent": True}])],
            showactive=False
        )],
        sliders=[dict(
            steps=[dict(method="animate", args=[[str(k)], dict(mode="immediate", frame=dict(duration=100, redraw=True), transition=dict(duration=0))], label=f"Step {k*SNAPSHOT_INTERVAL}") for k in range(n_snapshots)],
            transition=dict(duration=0),
            x=0,
            y=0,
            currentvalue=dict(font=dict(size=16), prefix="Snapshot: ", visible=True, xanchor="center"),
            len=1.0
        )]
    )

    fig.show()

if __name__ == '__main__':
    snapshots, n_snapshots = read_snapshots(filename_with_perturb)
    plot_with_slider(snapshots, n_snapshots)
