#pragma once

namespace PureAtelier
{
class GraphicsDevice
{
public:
    GraphicsDevice(GraphicsDevice&) = delete;
    GraphicsDevice& operator=(const GraphicsDevice&) = delete;
    static GraphicsDevice& GetInstance();

    void Initialize();
    void CreateCommittedResource(std::size_t bufferSize, Microsoft::WRL::ComPtr<ID3D12Resource>& resource);

private:
    Microsoft::WRL::ComPtr<ID3D12Device> m_D3D12Device;

    GraphicsDevice() = default;
    ~GraphicsDevice() = default;

    static Microsoft::WRL::ComPtr<IDXGIAdapter4> GetAdapter();
};
}
