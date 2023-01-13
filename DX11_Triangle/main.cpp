// #include "meshSimplify.h"
// #include "config.h"
// #include "matrix.h"
// #include "vector4.h"
// #include "solve.h"
// #include <iostream>
// #include <time.h>
// using namespace std;
//
// int main(int argc, char* argv[]) {
// 	if (argc != 4) {
// 		cout << "error input " << endl;
// 		return -1;
// 	}
// 	else {
// 		clock_t start = clock();//计时开始
//
// 		MeshSimplify* meshSimplify = new MeshSimplify();
// 		freopen(argv[1], "r", stdin);
// 		freopen(argv[2], "w", stdout);
// 		meshSimplify->setRatio(atof(argv[3]));//设置简化率
// 		meshSimplify->input();//读入
// 		meshSimplify->start();//开始简化
// 		meshSimplify->output();//输出
//
// 		clock_t finish = clock();//计时结束，计算简化时间
// 		cout << "# time use: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;
// 	}
// 	return 0;
// }

 #include "def.h"

 #include <stdio.h>
 #include <time.h>
 #include <assert.h>

 #define WIN32_LEAN_AND_MEAN
 #include <tchar.h>
 #include <Windows.h>
 #include <dxgi.h>
 #include <d3d11.h>
 #include <D3Dcompiler.h>
 #include <DirectXMath.h>
 // #include "d3dx11.h"
 #include <iostream>
 #include <shellapi.h>

 #include <string>
 #include <wrl.h>
 #include "resource.h"


 #include <vector>
 #include <cmath>
 // #include "tgaimage.h"
 #include "model.h"
 #include "geometry.h"
 #include "objload.h"
 #include <random>


 float generate()
 {
 	std::random_device dev;
 	std::mt19937 rng(dev());
 	std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100); // distribution in range [1, 6]

 	// std::cout << dist100(rng)/100 << std::endl;
 	return dist100(rng) / 100.f;
 }

 // const TGAColor white = TGAColor(255, 255, 255, 255);
 // const TGAColor red = TGAColor(255, 0, 0, 255);
 Model* model = NULL;
 const int width = 800;
 const int height = 800;
 size_t POINTS_NUM;
 size_t INDICES_NUM;
 /*
 #include <iostream>
 #include <cctype>
 #include <random>

 using u32 = uint_least32_t;
 using engine = std::mt19937;

 int main(void)
 {
 	std::random_device os_seed;
 	const u32 seed = os_seed();

 	engine generator(seed);
 	std::uniform_int_distribution< u32 > distribute(1, 6);

 	for (int repetition = 0; repetition < 10; ++repetition)
 		std::cout << distribute(generator) << std::endl;
 	return 0;
 }
 */
 // void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
 // {
 // 	bool steep = false;
 // 	if (std::abs(x0 - x1) < std::abs(y0 - y1))
 // 	{
 // 		std::swap(x0, y0);
 // 		std::swap(x1, y1);
 // 		steep = true;
 // 	}
 // 	if (x0 > x1)
 // 	{
 // 		std::swap(x0, x1);
 // 		std::swap(y0, y1);
 // 	}
 //
 // 	for (int x = x0; x <= x1; x++)
 // 	{
 // 		float t = (x - x0) / (float)(x1 - x0);
 // 		int y = y0 * (1. - t) + y1 * t;
 // 		if (steep)
 // 		{
 // 			image.set(y, x, color);
 // 		}
 // 		else
 // 		{
 // 			image.set(x, y, color);
 // 		}
 // 	}
 // }

 // int main(int argc, char** argv) {
 //     if (2 == argc) {
 //         model = new Model(argv[1]);
 //     }
 //     else {
 //         model = new Model("../DX11_Triangle/obj/african_head.obj");
 //     }
 //     std::cout << model->nfaces() << " " << model->nverts();
 //     TGAImage image(width, height, TGAImage::RGB);
 //     for (int i = 0; i < model->nfaces(); i++) {
 //         std::vector<int> face = model->face(i);
 //         for (int j = 0; j < 3; j++) {
 //             Vec3f v0 = model->vert(face[j]);
 // 			std::cout << v0.x << " " << v0.y << " " << v0.z<<std::endl;
 //             Vec3f v1 = model->vert(face[(j + 1) % 3]);
 //             int x0 = (v0.x + 1.) * width / 2.;
 //             int y0 = (v0.y + 1.) * height / 2.;
 //             int x1 = (v1.x + 1.) * width / 2.;
 //             int y1 = (v1.y + 1.) * height / 2.;
 //             line(x0, y0, x1, y1, image, white);
 //         }
 //     }
 //
 //     // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
 //     // image.write_tga_file("output.tga");
 //     delete model;
 //     return 0;
 // }
 #define First

 #ifdef First

 LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

 int windowWidth, windowHeight;
 HINSTANCE hInstance;
 HWND hWnd;
 float rot = 0;

 struct Vertex
 {
 	DirectX::XMFLOAT3 position;
 	DirectX::XMFLOAT4 color;
 };

 // Буфер констант, используемый для отправки матриц MVP в шейдер вершин.
 struct MVPConstantBuffer
 {
 	// DirectX::XMMATRIX mat;
 	// DirectX::XMFLOAT4X4 mat;
 	DirectX::XMFLOAT4X4 model;
 	DirectX::XMFLOAT4X4 view;
 	DirectX::XMFLOAT4X4 projection;
 };


 void InitWindows()
 {
 	// Получаем информацию о локальном модуле
 	hInstance = GetModuleHandle(NULL);
 	windowWidth = 640;
 	windowHeight = 480;

 	WNDCLASS wc;
 	// Без дополнительных параметров
 	wc.style = 2;
 	// Задаем обработчик событий
 	wc.lpfnWndProc = &WndProc;
 	// Нам не нужно дополнительное выделение памяти к структуре окна и структуре класса
 	wc.cbClsExtra = 0;
 	wc.cbWndExtra = 0;
 	// Модуль (программа), которому принадлежит обработчик событий
 	wc.hInstance = hInstance;
 	// Загружаем стандартный курсор и стандартную иконку
 	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
 	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
 	// Цвет фона не важен, поэтому не задаем "кисть"
 	wc.hbrBackground = NULL;
 	// Окно без меню
 	wc.lpszMenuName = NULL;
 	// Задаем название класса окна
 	wc.lpszClassName = _T("Window");

 	// Регистрируем класс окна
 	ATOM result = RegisterClass(&wc);
 	// Проверяем, что класс успешно зарегистрирован
 	assert(result);

 	// Стандартное окно -- имеет заголовок, можно изменить размер и т.д.
 	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
 	RECT rect;
 	// Клиентская область (внутри рамки) по центру экрана и заданного размера
 	rect.left = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
 	rect.top = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
 	rect.right = rect.left + windowWidth;
 	rect.bottom = rect.top + windowHeight;
 	// Вычислим область окна с рамкой. Последний параметр -- наличие меню
 	AdjustWindowRect(&rect, dwStyle, FALSE);

 	hWnd = CreateWindow(
 		_T("Window"),
 		_T("Triangle"),
 		dwStyle,
 		// Левый верхний угол окна
 		rect.left, rect.top,
 		// Размер окна
 		rect.right - rect.left,
 		rect.bottom - rect.top,
 		// Родительское окно
 		// HWND_DESKTOP раскрывается в NULL
 		HWND_DESKTOP,
 		// Меню
 		NULL,
 		// Модуль (программа), которой принадлежит окно
 		hInstance,
 		// Дополнительные свойства
 		NULL);
 	// Проверяем, что окно успешно создано
 	assert(hWnd);
 }

 void DisposeWindows()
 {
 	// Удаляем окно
 	DestroyWindow(hWnd);
 	// Удаляем класс
 	UnregisterClass(_T("Triangle"), hInstance);
 }

 IDXGISwapChain* swapChain;
 ID3D11Device* device;
 ID3D11DeviceContext* deviceContext;

 ID3D11SamplerState* samplerState;
// TODO: 
 ID3D11ShaderResourceView* myTexture;

 ID3D11SamplerState* samplerState1;
 // TODO: 
 ID3D11ShaderResourceView* myTexture1;

 ID3D11SamplerState* samplerState2;
 // TODO: 
 ID3D11ShaderResourceView* myTexture2;

 ID3D11SamplerState* samplerState3;
 // TODO: 
 ID3D11ShaderResourceView* myTexture3;

void initSamplerState()
{
    HRESULT result;
    //Create sampler description for sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    result = device->CreateSamplerState(&sampDesc, &samplerState); //Create sampler state

    // Проверяем, что операция прошла успешно
    assert(SUCCEEDED(result));

    //1
    result = device->CreateSamplerState(&sampDesc, &samplerState1); //Create sampler state

    // Проверяем, что операция прошла успешно
    assert(SUCCEEDED(result));

    //2
    result = device->CreateSamplerState(&sampDesc, &samplerState2); //Create sampler state

    // Проверяем, что операция прошла успешно
    assert(SUCCEEDED(result));

    //3
    result = device->CreateSamplerState(&sampDesc, &samplerState3); //Create sampler state

    // Проверяем, что операция прошла успешно
    assert(SUCCEEDED(result));
}
 void InitSwapChain()
 {
 	HRESULT result;

 	DXGI_SWAP_CHAIN_DESC swapChainDesc;
 	// Разрмер совпадает с размером клиентской части окна
 	swapChainDesc.BufferDesc.Width = windowWidth;
 	swapChainDesc.BufferDesc.Height = windowHeight;
 	// Ограничение количества кадров в секунду задается в виде рационального числа
 	// Т.к. нам нужна максимальная частота кадров, отключаем
 	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
 	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
 	// Формат вывода -- 32-битный RGBA
 	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
 	// Не задаем масштабирования при выводе
 	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 	// Не используем сглаживание
 	swapChainDesc.SampleDesc.Count = 1;
 	swapChainDesc.SampleDesc.Quality = 0;
 	// Используем SwapChain для вывода
 	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
 	// Один "задний" (не отображаемый) буфер
 	swapChainDesc.BufferCount = 1;
 	// Задаем окно для вывода
 	swapChainDesc.OutputWindow = hWnd;
 	// Оконный режим
 	swapChainDesc.Windowed = TRUE;
 	// Отбрасываем старую информацию из буфера при выводе на экран
 	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
 	swapChainDesc.Flags = 0;

 	// Используем DirectX 11.0, т.к. его нам достаточно
 	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

 	// В Debug-версии создаем поток отладки DirectX
 #ifndef NDEBUG
 	UINT flags = D3D11_CREATE_DEVICE_DEBUG;
 #else
 	UINT flags = 0;
 #endif

 	result = D3D11CreateDeviceAndSwapChain(
 		// Используем видеоадаптер по-умолчанию
 		NULL,
 		// Аппаратная реализация
 		D3D_DRIVER_TYPE_HARDWARE, NULL,
 		flags,
 		// Используем одну версию DirectX
 		&featureLevel, 1,
 		// Версия SDK
 		D3D11_SDK_VERSION,
 		// Передаем созданное ранее описание
 		&swapChainDesc,
 		// Указатели, куда записать результат
 		&swapChain, &device, NULL, &deviceContext);
 	// Проверяем, что операция прошла успешно
 	assert(SUCCEEDED(result));
 }

 void DisposeSwapChain()
 {
     deviceContext->Release();
     device->Release();
     swapChain->Release();
 }
 void DisposeSamplerState()
 {
     samplerState->Release();
     samplerState1->Release();
     samplerState2->Release();
     samplerState3->Release();
 }
 ID3D11RenderTargetView* renderTargetView;

 void InitRenderTargetView()
 {
 	HRESULT result;
 	ID3D11Texture2D* backBuffer;

 	// Берем "задний" буфер из SwapChain
 	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
 	assert(SUCCEEDED(result));

 	// Инициализируем доступ к буферу для записи и для отрисовки
 	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
 	assert(SUCCEEDED(result));

 	// Указатель на буфер больше нам не нужен
 	// Стоит отметить, что сам буфер при этом не удаляется,
 	// т.к. на него всё ещё указывает SwapChain,
 	// Release() лишь освобождает указатель
 	backBuffer->Release();

 	// Используем созданный View для отрисовки
 	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

 	// Задаем область отрисовки
 	D3D11_VIEWPORT viewport;
 	viewport.TopLeftX = 0;
 	viewport.TopLeftY = 0;
 	viewport.Width = (FLOAT)windowWidth;
 	viewport.Height = (FLOAT)windowHeight;
 	viewport.MinDepth = 0;
 	viewport.MaxDepth = 1;
 	deviceContext->RSSetViewports(1, &viewport);
 }

 void DisposeRenderTargetView()
 {
 	renderTargetView->Release();
 }


 ID3D11VertexShader* vertexShader;
 ID3D11GeometryShader* geometryShader;
 ID3D11ComputeShader* computeShader;
 ID3D11PixelShader* pixelShader;
 ID3D11InputLayout* inputLayout;

 void InitShaders()
 {
 	HRESULT result;
 	HRSRC src;
 	HGLOBAL res;


 #if defined(_DEBUG)
 	// Enable better shader debugging with the graphics debugging tools.
 	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
 #else
 	UINT compileFlags = 0;
 #endif

    // Инициализация вычислительного шейдера
// Берем встроенные в исполняемый файл ресурсы
// Проверка ошибок не производится, т.к. байткод расположен внутри
// исполняемого файла и не может быть не найден
    {
        src = FindResource(hInstance, MAKEINTRESOURCE(IDR_BYTECODE_COMPUTE), _T("ShaderObject"));
        res = LoadResource(hInstance, src);
        // Инициализируем шейдер
        result = device->CreateComputeShader(
            // Байткод шейдера и его размер
            res, SizeofResource(hInstance, src),
            // Свойства для компоновщика. В нашем случае не используется, т.к. шейдер из одного объекта
            NULL,
            // Указатель на шейдер
            &computeShader);
        assert(SUCCEEDED(result));
        FreeResource(res);
    }
    {
        // Пиксельный шейдера
        src = FindResource(hInstance, MAKEINTRESOURCE(IDR_BYTECODE_PIXEL), _T("ShaderObject"));
        res = LoadResource(hInstance, src);

        result = device->CreatePixelShader(res, SizeofResource(hInstance, src),
            NULL, &pixelShader);
        assert(SUCCEEDED(result));
        FreeResource(res);
    }
    {
        // Шейдер геометрии
        src = FindResource(hInstance, MAKEINTRESOURCE(IDR_BYTECODE_GEOMETRY), _T("ShaderObject"));
        res = LoadResource(hInstance, src);
        result = device->CreateGeometryShader(res, SizeofResource(hInstance, src),
            NULL, &geometryShader);
        assert(SUCCEEDED(result));
        FreeResource(res);
    }
    {
        // Аналогично для вершинного шейдера
        src = FindResource(hInstance, MAKEINTRESOURCE(IDR_BYTECODE_VERTEX), _T("ShaderObject"));
        res = LoadResource(hInstance, src);
        result = device->CreateVertexShader(res, SizeofResource(hInstance, src),
            NULL, &vertexShader);
        assert(SUCCEEDED(result));
    }
 	//Задаем, как в вершинный шейдер будут вводиться данные
 	//Описание параметров

 	// Семантическое имя аргумента
 	// Нужно только в случае, если элементов с данным семантическим именем больше одного
 	// Двумерный вектор из 32-битных вещественных чисел
 	// Первый параметр
 	// Необязательный аргумент
 	// Для каждой вершины
 	// Используем вершины для отрисовки
 	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
 	{
 		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
 		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
 	};

 	UINT num_of_el = ARRAYSIZE(inputDesc);
 	result = device->CreateInputLayout(
 		// Массив описаний аргументов и его длина
 		inputDesc, num_of_el,
 		// Байткод и его длина
 		res, SizeofResource(hInstance, src),
 		// Структура ввода
 		&inputLayout);
 	assert(SUCCEEDED(result));

 	FreeResource(res);
 }

 void DisposeShaders()
 {
 	inputLayout->Release();
 	vertexShader->Release();
    geometryShader->Release();
    computeShader->Release();
 	pixelShader->Release();
 }

 Microsoft::WRL::ComPtr<ID3D11Buffer> positionBuffer;
 Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
 Microsoft::WRL::ComPtr<ID3D11Buffer> indicesBuffer;

 // Инициализирует параметры представления при изменении размера окна.
 MVPConstantBuffer data;
 static DirectX::XMVECTORF32 eye = {0.0f, 0.0f, 1.0f, 0.0f};
 static DirectX::XMVECTORF32 at = {0.0f, -0.1f, 0.0f, 0.0f}; //Look at center of the world
 void UpdateMVP()
 {
 	// пропорция размеров окна
 	float aspectRatio = windowWidth / windowHeight;
 	// угол обзора
 	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

 	// Это простой пример изменения, которое можно реализовать, когда приложение находится в
 	// книжном или прикрепленном представлении.
 	if (aspectRatio < 1.0f)
 	{
 		fovAngleY *= 2.0f;
 	}

 	// Gравая система координат на базе матриц с развертыванием по столбцам.
 	DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
 		fovAngleY,
 		aspectRatio,
 		0.01f,
 		100.0f
 	);

 	DirectX::XMFLOAT4X4 orientation(
 		1.0f, 0.0f, 0.0f, 0.0f,
 		0.0f, 1.0f, 0.0f, 0.0f,
 		0.0f, 0.0f, 1.0f, 0.0f,
 		0.0f, 0.0f, 0.0f, 1.0f
 	);

 	DirectX::XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);


 	float fovDegrees = 90.0f; //90 Degree Field of View
 	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
 	float nearZ = 0.1f;
 	float farZ = 1000.0f;
 	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

 	XMStoreFloat4x4(
 		&data.projection,
 		DirectX::XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
 	);


 	// Наблюдатель находится в точке eye и смотрит в точку at, а вектор вертикали направлен вдоль оси Y.

 	static const DirectX::XMVECTORF32 up = {0.0f, 1.0f, 0.0f, 0.0f}; //Positive Y Axis = Up

 	XMStoreFloat4x4(&data.view, XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up)));
 	// Изменяем данны структуры
 	D3D11_MAPPED_SUBRESOURCE MappedResource;
 	HRESULT hr = deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
 	                                &MappedResource);
 	CopyMemory(MappedResource.pData, &data, sizeof(MVPConstantBuffer));
 	deviceContext->Unmap(constBuffer.Get(), 0);
 }


 void InitBuffers()
 {
 	HRESULT result;


 	model = new Model("../DX11_Triangle/obj/dragon.obj");
 	std::vector<Vertex> triangleVertices;
 	std::vector<DWORD> indices;

 	for (int i = 0; i < model->nverts(); i++)
 	{
 		// colo
 		Vertex v = {
 			{model->vert(i).x, model->vert(i).y, model->vert(i).z}, {generate(), generate(), generate(), 1.0f}
 		};
 		// std::cout << model->vert(i).x << " " << model->vert(i).y << " " << model->vert(i).z << " " << std::endl;
 		triangleVertices.push_back(v);
 	}
 	std::cout << std::endl;
 	for (int i = 0; i < model->nfaces(); i++)
 	{
 		generate();
 		std::vector<int> face = model->face(i);
 		// std::cout << face.size() << std::endl;
 		for (int j = 0; j < 3; j++)
 		{
 			indices.push_back(face[2-j]-1);
 			  // std::cout << face[j] << " ";
 		}

 		   // std::cout << std::endl;
 	}


 	// Определяем геометрию объекта
 	// Vertex triangleVertices[] =
 	// {
 	// 	{{-0.25f, -0.25f * 1, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, //[0]
 	// 	{{-0.25f, 0.25f * 1, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, //[1]
 	// 	{{0.25f, 0.25f * 1, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, //[2]
 	// 	{{0.1f, 0.1f * 1, -0.1f}, {1.0f, 1.0f, 0.0f, 1.0f}} //[3]
 	// };


 	const UINT vertexBufferSize = sizeof(triangleVertices[0]) * triangleVertices.size();
 	POINTS_NUM = triangleVertices.size()*3*2;
    INDICES_NUM = indices.size();// количество точек
 	// Описание массива, который будет записан в буфер при создании
 	D3D11_SUBRESOURCE_DATA subresource;
 	// Указатель на массив
 	subresource.pSysMem = reinterpret_cast<UINT8*>(triangleVertices.data());
 	{
 		// Описание буфера
 		D3D11_BUFFER_DESC desc;
 		// Его размер
 		desc.ByteWidth = vertexBufferSize;
 		// Доступ на чтение и запись
 		desc.Usage = D3D11_USAGE_DEFAULT;
 		// Буфер позиций используем и для отрисовки, и при вычислениях как массив
 		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
 		// Доступ с процессора не нужен
 		desc.CPUAccessFlags = 0;
 		// Дополнительные флаги не нужны
 		desc.MiscFlags = 0;
 		// Размер одного элемента буфера
 		desc.StructureByteStride = sizeof(Vertex);

 		// Создаем буфер позиций
 		result = device->CreateBuffer(&desc, &subresource, positionBuffer.GetAddressOf());
 		assert(SUCCEEDED(result));
 	}


 	// DWORD indices[] =
 	//
 	// {
 	// 	0, 1, 2,
 	// 	2, 1, 0
 	// 	// 3, 2, 0,
 	// 	// 3,2,1
 	// };

 	// std::cout << sizeof(indices[0]) * indices.size();
 	{
 		// Описание буфера индексов
 		D3D11_BUFFER_DESC indexdesc;
 		ZeroMemory(&indexdesc, sizeof(indexdesc));
 		// Его размер
 		indexdesc.ByteWidth = sizeof(DWORD) * indices.size(); //ARRAYSIZE((indices.data()));
 		// Доступ на чтение и запись
 		indexdesc.Usage = D3D11_USAGE_DEFAULT;
 		// Буфер индексов позиций 
 		indexdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
 		// Доступ с процессора не нужен
 		indexdesc.CPUAccessFlags = 0;
 		// Дополнительные флаги не нужны
 		indexdesc.MiscFlags = 0;

 		D3D11_SUBRESOURCE_DATA indexBufferData;
 		indexBufferData.pSysMem = indices.data();
 		// Создаем буфер позиций
 		result = device->CreateBuffer(&indexdesc, &indexBufferData, indicesBuffer.GetAddressOf());
 		assert(SUCCEEDED(result));
 	}

 	{
 		// Описание константного буфера
 		D3D11_BUFFER_DESC desc;
 		// Его размер
 		const UINT constBufferSize = sizeof(triangleVertices);
 		// Выравниваем буфер
 		desc.ByteWidth = static_cast<UINT>(constBufferSize + (16 - (constBufferSize % 16)));
 		// Доступ на чтение и запись
 		desc.Usage = D3D11_USAGE_DYNAMIC;
 		// Буфер позиций используем и для отрисовки, и при вычислениях как массив
 		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 		// Доступ с процессора не нужен
 		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 		// Дополнительные флаги не нужны
 		desc.MiscFlags = 0;
 		// Размер одного элемента буфера
 		desc.StructureByteStride = sizeof(MVPConstantBuffer);
 		// Создаем буфер матриц поворота
 		result = device->CreateBuffer(&desc, 0, constBuffer.GetAddressOf());
 		assert(SUCCEEDED(result));
 	}


    {
        // D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
        // vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        // vertexBufferDesc.ByteWidth = (unsigned int)(sizeof(m.vertices[0]) * m.vertexCount);
        // vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        // vertexBufferDesc.StructureByteStride = sizeof(m.vertices[0]);
        // vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        // vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        // vertexBufferDesc.CPUAccessFlags = 0;
        //
        // D3D11_SUBRESOURCE_DATA vertexBufferData = { m.vertices, 0, 0 };
        // result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

        // D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        // //ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        // shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        // shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
        // shaderResourceViewDesc.Buffer.FirstElement = 0;
        // shaderResourceViewDesc.Buffer.NumElements = 1;
        //
        // hr = device->CreateShaderResourceView(vertexBuffer, &shaderResourceViewDesc, &particleResourceView);
    }
 }

 void DisposeBuffers()
 {
 	delete model;
 }

// Вертексы
 ID3D11UnorderedAccessView* positionUAV;
// Индексы
 ID3D11UnorderedAccessView* indicesUAV;
 // ID3D11UnorderedAccessView* velocityUAV;
 void InitUAV()
 {
 	HRESULT result;

 	// Описание доступа к буферу из шейдера как к массиву
 	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
 	// Двумерный вектор из 32-битных вещественных чисел
 	desc.Format = DXGI_FORMAT_R32G32_FLOAT;
 	// Доступ к буферу, есть также варианты с текстурами
 	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
 	// Доступ с первого элемента
 	desc.Buffer.FirstElement = 0;
 	// Количество элементов
 	//TODO: Change to size of indices
 	desc.Buffer.NumElements = POINTS_NUM;
 	// Без дополнительных флагов
 	desc.Buffer.Flags = 0;

 	// Инициализация доступа к буферу позиций
 	result = device->CreateUnorderedAccessView(positionBuffer.Get(), &desc,
 	                                           &positionUAV);



    // Двумерный вектор из 32-битных вещественных чисел
    desc.Format = DXGI_FORMAT_R32G32_FLOAT;
    // Доступ к буферу, есть также варианты с текстурами
    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    // Доступ с первого элемента
    desc.Buffer.FirstElement = 0;
    // Количество элементов
    //TODO: Change to size of indices
    desc.Buffer.NumElements = INDICES_NUM;
    // Без дополнительных флагов
    desc.Buffer.Flags = 0;

    // Инициализация доступа к буферу индексов
    result = device->CreateUnorderedAccessView(indicesBuffer.Get(), &desc,
        &indicesUAV);
    // assert(!result);
    //TODO:
 	// assert(!result);
 }

 void DisposeUAV()
 {
 	positionUAV->Release();
    indicesUAV->Release();
 }

 void InitBindings()
 {
 	// Устанавливаем используемые шейдеры
    // Пиксельный сэмплер
     deviceContext->PSSetSamplers(0, 1, &samplerState);
     // Вычислительный
     deviceContext->CSSetShader(computeShader, NULL, 0);

 	// Вершинный
 	deviceContext->VSSetShader(vertexShader, NULL, 0);

    // Шейдер геометрии
    deviceContext->GSSetShader(geometryShader, NULL, 0);
 	// Пиксельный
 	deviceContext->PSSetShader(pixelShader, NULL, 0);


 	// Подготовка буфера констант для передачи графическому устройству.
 	deviceContext->UpdateSubresource(
 		constBuffer.Get(),
 		0,
 		NULL,
 		&data,
 		0,
 		0
 	);


 	// Устанавливаем способ записи аргументов вершинного шейдера
 	deviceContext->IASetInputLayout(inputLayout);
 	// Рисовать будем точки
 	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 }

 // Поворот модели  на заданный угол в радианах.
 void Rotate(float radians)
 {
 	// Подготовка к передаче обновленной матрицы модели шейдеру
 	DirectX::XMStoreFloat4x4(&data.model, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(radians)));
 }


 void TrackingUpdate(float positionX)
 {
 	float radians = DirectX::XM_2PI * 2.0f * positionX / windowWidth;
 	Rotate(radians);
 }
 const int FRAME_TIME_COUNT = 128;
 clock_t frameTime[FRAME_TIME_COUNT];
 int currentFrame = 0;

 float AverageFrameTime()
 {
     frameTime[currentFrame] = clock();
     int nextFrame = (currentFrame + 1) % FRAME_TIME_COUNT;
     clock_t delta = frameTime[currentFrame] - frameTime[nextFrame];
     currentFrame = nextFrame;
     return (float)delta / CLOCKS_PER_SEC / FRAME_TIME_COUNT;
 }


/*#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>

 // Replace with your own vertex structure.
 struct Vertex
 {
     float position[3];
     float color[4];
 };

 // Replace with your own device and context pointers.
 ID3D11Device* pDevice = nullptr;
 ID3D11DeviceContext* pContext = nullptr;

 // Replace with your own compute shader file name and entry point.
 const char* computeShaderFile = "MyComputeShader.hlsl";
 const char* computeShaderEntryPoint = "MyComputeShaderFunction";

 // Create a vertex buffer to hold your vertices.
 std::vector<Vertex> vertices = { /* Fill with vertices #1# };
 ID3D11Buffer* pVertexBuffer = nullptr;
 D3D11_BUFFER_DESC vertexBufferDesc = {};
 vertexBufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
 vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
 vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
 D3D11_SUBRESOURCE_DATA vertexBufferData = {};
 vertexBufferData.pSysMem = vertices.data();
 HRESULT hr = pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &pVertexBuffer);
 if (FAILED(hr))
 {
     // Handle error.
 }

 // Load and compile the compute shader.
 ID3DBlob* pComputeShaderBlob = nullptr;
 hr = D3DCompileFromFile(computeShaderFile, nullptr, nullptr, computeShaderEntryPoint, "cs_5_0", 0, 0, &pComputeShaderBlob, nullptr);
 if (FAILED(hr))
 {
     // Handle error.
 }

 // Create the compute shader from the compiled blob.
 ID3D11ComputeShader* pComputeShader = nullptr;
 hr = pDevice->CreateComputeShader(pComputeShaderBlob->GetBufferPointer(), pComputeShaderBlob->GetBufferSize(), nullptr, &pComputeShader);
 if (FAILED(hr))
 {
     // Handle error.
 }

 // Set the compute shader and vertex buffer as the current pipeline state.
 pContext->CSSetShader(pComputeShader, nullptr, 0);
 pContext->CSSetConstantBuffers(0, 1, &pVertexBuffer);

 // Dispatch the compute shader.
 pContext->Dispatch(/* Thread groups X #1#, /* Thread groups Y #1#, /* Thread groups Z #1#);

 // Reset the pipeline state.
 pContext->CSSetShader(nullptr, nullptr, 0);
 pContext->CSSetConstantBuffers(0, 1, nullptr);

 // Release the resources.
 pComputeShader->Release();
 pComputeShaderBlob->Release();
 pVertexBuffer->Release();*/
 void Frame()
 {
     float frameTime = AverageFrameTime();

     // Выводим фреймрейт
     char buf[256];
     sprintf_s(buf, "average framerate: %.1f", 1.0f / frameTime);
     SetWindowTextA(hWnd, buf);

 	// Очищаем буфер 
 	const float clearColor[] = {0.2f, 0.2f, 0.4f, 1.0f};
 	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
 	//TODO
 	// Двумерные вектора из 32-битных вещественных идут подряд
 	UINT stride = sizeof(Vertex); // sizeof(float[4]);
 	UINT offset = 0;

 	ID3D11Buffer* nullBuffer = NULL;
 	ID3D11UnorderedAccessView* nullUAV = NULL;

 	//Update Constant Buffer
 	TrackingUpdate(rot);
 	UpdateMVP();
    // Устанавливаем доступ вычислительного шейдера к буферу позиций
// deviceContext->CSSetConstantBuffers(0, 1, positionBuffer.GetAddressOf());
    deviceContext->CSSetUnorderedAccessViews(1, 1, &indicesUAV, NULL);

    // ID3D11UnorderedAccessView* srvs[2] = { positionUAV, indicesUAV };
    // Убираем доступ вершинного шейдера к буферу позиций
    deviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
    // Убираем доступ
    deviceContext->IASetIndexBuffer(nullBuffer, DXGI_FORMAT_R32_UINT, 0);



    // Устанавливаем доступ вычислительного шейдера к буферу индексов
    deviceContext->CSSetUnorderedAccessViews(0, 1, &positionUAV, NULL);

    // Вызываем вычислительный шейдер
    deviceContext->Dispatch(POINTS_NUM / NUMTHREADS, 1, 1);

    // // Убираем доступ вычислительного шейдера к буферу позиций
    // deviceContext->CSSetShader(NULL, NULL, 0);
    // deviceContext->CSSetConstantBuffers(0, 1, NULL);
    deviceContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, NULL);
    // Убираем доступ вычислительного шейдера к буферу индексов
    // deviceContext->CSSetUnorderedAccessViews(1, 2, &nullUAV, NULL);

    // Устанавливаем доступ вершинного шейдера к константному буферу
 	deviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
 	// Устанавливаем доступ вершинного шейдера к буферу позиций
 	deviceContext->IASetVertexBuffers(0, 1, positionBuffer.GetAddressOf(), &stride, &offset);
 	deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
 	// Вызываем отрисовку
 	// deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
 	deviceContext->DrawIndexed(POINTS_NUM, 0, 0);
 	// Выводим изображение на экран
 	swapChain->Present(0, 0);
 }

 void ResizeSwapChain()
 {
 	HRESULT result;

 	RECT rect;
 	// Получаем актуальные размеры окна
 	GetClientRect(hWnd, &rect);
 	windowWidth = rect.right - rect.left;
 	windowHeight = rect.bottom - rect.top;

 	// Для того, чтобы изменить размер изображения, нужно
 	// освободить все указатели на "задний" буфер
 	DisposeRenderTargetView();
 	// Изменяем размер изображения
 	result = swapChain->ResizeBuffers(
 		// Изменяем размер всех буферов
 		0,
 		// Новые размеры
 		windowWidth, windowHeight,
 		// Не меняем формат и флаги
 		DXGI_FORMAT_UNKNOWN, 0);
 	assert(SUCCEEDED(result));
 	// Создаем новый доступ к "заднему" буферу
 	InitRenderTargetView();
 }

 float rot_eye = 0;

 LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
 {
 	unsigned char ch = static_cast<unsigned char>(wParam);
 	switch (Msg)
 	{
 	case WM_CHAR:
 		{
 			//std::cout << char(wParam);
 			if (ch == 'a')
 			{
 				at.f[0] += 0.05;
 				eye.f[0] += 0.05;
 				/*float dx = at.f[0] - eye.f[0];
 				float new_x_at = at.f[0] - 0.05;
 				at.f[2] = new_x_at * eye.f[2] / at.f[0];
 				float new_x_eye = eye.f[0] - 0.05;
 				eye.f[2] = new_x_eye * at.f[2] / eye.f[0];*/
 			}
 			if (ch == 'd')
 			{
 				at.f[0] -= 0.05;
 				eye.f[0] -= 0.05;
 			}
 			if (ch == 'w')
 			{
 				at.f[1] += 0.05;
 				eye.f[1] += 0.05;
 			}
 			if (ch == 's')
 			{
 				at.f[1] -= 0.05;
 				eye.f[1] -= 0.05;
 			}

 			if (ch == 'q')
 			{
 				// Вектор перемещения
 				float dx = at.f[0] - eye.f[0];
 				float dz = at.f[2] - eye.f[2];

 				float new_dx = dx * 1.25;
 				float new_dz = dz * 1.25;
 				at.f[0] = at.f[0] + new_dx - dx;
 				at.f[2] = at.f[2] + new_dz - dz;
 				eye.f[0] = eye.f[0] + new_dx - dx;
 				eye.f[2] = eye.f[2] + new_dz - dz;
 				/*	at.f[2] -= 0.05f;
 					eye.f[2] -= 0.05f;*/
 			}
 			if (ch == 'e')
 			{
 				// Вектор перемещения
 				float dx = at.f[0] - eye.f[0];
 				float dz = at.f[2] - eye.f[2];

 				float new_dx = dx / 1.25;
 				float new_dz = dz / 1.25;
 				at.f[0] = at.f[0] + new_dx - dx;
 				at.f[2] = at.f[2] + new_dz - dz;
 				eye.f[0] = eye.f[0] + new_dx - dx;
 				eye.f[2] = eye.f[2] + new_dz - dz;
 			}

 			// Вращение камеры относительно своей координаты
 			if (ch == 'l')
 			{
 				rot_eye = 0.05f;
 				float dx = at.f[0] - eye.f[0];
 				float dz = at.f[2] - eye.f[2];
 				at.f[0] = eye.f[0] + dx * cos(rot_eye) - dz * sin(rot_eye);
 				at.f[2] = eye.f[2] + dx * sin(rot_eye) + dz * cos(rot_eye);
 			}
 			if (ch == 'j')
 			{
 				rot_eye = -0.05f;
 				float dx = at.f[0] - eye.f[0];
 				float dz = at.f[2] - eye.f[2];
 				at.f[0] = eye.f[0] + dx * cos(rot_eye) - dz * sin(rot_eye);
 				at.f[2] = eye.f[2] + dx * sin(rot_eye) + dz * cos(rot_eye);
 			}
 			// Вращение камеры вокруг предмета
 			if (ch == 'i')
 			{
 				rot_eye = 0.05f;
 				float tmp_x = eye.f[0];
 				float tmp_z = eye.f[2];
 				eye.f[0] = tmp_x * cos(rot_eye) - tmp_z * sin(rot_eye);
 				eye.f[2] = tmp_x * sin(rot_eye) + tmp_z * cos(rot_eye);
 			}
 			if (ch == 'k')
 			{
 				rot_eye = -0.05f;
 				float tmp_x = eye.f[0];
 				float tmp_z = eye.f[2];
 				eye.f[0] = tmp_x * cos(rot_eye) - tmp_z * sin(rot_eye);
 				eye.f[2] = tmp_x * sin(rot_eye) + tmp_z * cos(rot_eye);
 			}

 			break;
 		}
 	case WM_CLOSE:
 		PostQuitMessage(0);
 		break;

 	case WM_KEYDOWN:
 		if (wParam == VK_ESCAPE)
 			PostQuitMessage(0);
 		break;

 	case WM_SIZE:
 		ResizeSwapChain();
 		break;

 	default:
 		return DefWindowProc(hWnd, Msg, wParam, lParam);
 	}

 	return 0;
 }

 int main()
 {
 	InitWindows();
 	InitSwapChain();
    initSamplerState();
 	InitRenderTargetView();
 	InitShaders();
 	InitBuffers();
 	InitUAV();
 	InitBindings();

 	ShowWindow(hWnd, SW_SHOW);

 	bool shouldExit = false;
 	while (!shouldExit)
 	{
 		Frame();

 		MSG msg;
 		while (!shouldExit && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
 		{
 			TranslateMessage(&msg);
 			DispatchMessage(&msg);

 			if (msg.message == WM_QUIT)
 				shouldExit = true;
 		}
 	}

 	DisposeUAV();
 	DisposeBuffers();
 	DisposeShaders();

 	DisposeRenderTargetView();
 	DisposeSwapChain();
    DisposeSamplerState();
 	DisposeWindows();
 }


 #endif First


 #ifdef Second
 // include the basic windows header files and the Direct3D header files
 #include <windows.h>
 #include <windowsx.h>
 #include <d3d11.h>
 // #include <d3dx11.h>
 / #include <d3dx10.h>

 // include the Direct3D Library file
 #pragma comment (lib, "d3d11.lib")
 // #pragma comment (lib, "d3dx11.lib")
 / #pragma comment (lib, "d3dx10.lib")

 // define the screen resolution
 #define SCREEN_WIDTH  800
 #define SCREEN_HEIGHT 600

 // global declarations
 IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
 ID3D11Device* dev;                     // the pointer to our Direct3D device interface
 ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
 ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
 ID3D11InputLayout* pLayout;            // the pointer to the input layout
 ID3D11VertexShader* pVS;               // the pointer to the vertex shader
 ID3D11PixelShader* pPS;                // the pointer to the pixel shader
 ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

 // a struct to define a single vertex
 struct Vertex
 {
 	DirectX::XMFLOAT3 position;
 	DirectX::XMFLOAT4 color;
 };

 // function prototypes
 void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
 void RenderFrame(void);     // renders a single frame
 void CleanD3D(void);        // closes Direct3D and releases memory
 void InitGraphics(void);    // creates the shape to render
 void InitPipeline(void);    // loads and prepares the shaders

 // the WindowProc function prototype
 LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
 HINSTANCE Instance;

 // the entry point for any Windows program
 int WINAPI WinMain(HINSTANCE hInstance,
     HINSTANCE hPrevInstance,
     LPSTR lpCmdLine,
     int nCmdShow)
 {
     HWND hWnd;
     WNDCLASSEX wc;

     ZeroMemory(&wc, sizeof(WNDCLASSEX));
     Instance = hInstance;
     wc.cbSize = sizeof(WNDCLASSEX);
     wc.style = CS_HREDRAW | CS_VREDRAW;
     wc.lpfnWndProc = WindowProc;
     wc.hInstance = hInstance;
     wc.hCursor = LoadCursor(NULL, IDC_ARROW);
     wc.lpszClassName = "WindowClass";

     RegisterClassEx(&wc);

     RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
     AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

     hWnd = CreateWindowEx(NULL,
         "WindowClass",
         "Our First Direct3D Program",
         WS_OVERLAPPEDWINDOW,
         300,
         300,
         wr.right - wr.left,
         wr.bottom - wr.top,
         NULL,
         NULL,
         hInstance,
         NULL);

     ShowWindow(hWnd, nCmdShow);

     // set up and initialize Direct3D
     InitD3D(hWnd);

     // enter the main loop:

     MSG msg;

     while (TRUE)
     {
         if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
         {
             TranslateMessage(&msg);
             DispatchMessage(&msg);

             if (msg.message == WM_QUIT)
                 break;
         }

         RenderFrame();
     }

     // clean up DirectX and COM
     CleanD3D();

     return msg.wParam;
 }


 // this is the main message handler for the program
 LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
 {
     switch (message)
     {
     case WM_DESTROY:
     {
         PostQuitMessage(0);
         return 0;
     } break;
     }

     return DefWindowProc(hWnd, message, wParam, lParam);
 }


 // this function initializes and prepares Direct3D for use
 void InitD3D(HWND hWnd)
 {
     // create a struct to hold information about the swap chain
     DXGI_SWAP_CHAIN_DESC scd;

     // clear out the struct for use
     ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

     // fill the swap chain description struct
     scd.BufferCount = 1;                                   // one back buffer
     scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
     scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
     scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
     scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
     scd.OutputWindow = hWnd;                               // the window to be used
     scd.SampleDesc.Count = 4;                              // how many multisamples
     scd.Windowed = TRUE;                                   // windowed/full-screen mode
     scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

     // create a device, device context and swap chain using the information in the scd struct
     D3D11CreateDeviceAndSwapChain(NULL,
         D3D_DRIVER_TYPE_HARDWARE,
         NULL,
         NULL,
         NULL,
         NULL,
         D3D11_SDK_VERSION,
         &scd,
         &swapchain,
         &dev,
         NULL,
         &devcon);


     // get the address of the back buffer
     ID3D11Texture2D* pBackBuffer;
     swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

     // use the back buffer address to create the render target
     dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
     pBackBuffer->Release();

     // set the render target as the back buffer
     devcon->OMSetRenderTargets(1, &backbuffer, NULL);


     // Set the viewport
     D3D11_VIEWPORT viewport;
     ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

     viewport.TopLeftX = 0;
     viewport.TopLeftY = 0;
     viewport.Width = SCREEN_WIDTH;
     viewport.Height = SCREEN_HEIGHT;

     devcon->RSSetViewports(1, &viewport);

     InitPipeline();
     InitGraphics();
 }


 // this is the function used to render a single frame
 void RenderFrame(void)
 {
     // clear the back buffer to a deep blue
     const float clearColor[] = { 0.2f, 0.2f, 0.4f, 1.0f };
     devcon->ClearRenderTargetView(backbuffer, clearColor);

     // select which vertex buffer to display
     UINT stride = sizeof(Vertex);
     UINT offset = 0;
     devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

     // select which primtive type we are using
     devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

     // draw the vertex buffer to the back buffer
     devcon->Draw(POINTS_NUM, 0);

     // switch the back buffer and the front buffer
     swapchain->Present(0, 0);
 }


 // this is the function that cleans up Direct3D and COM
 void CleanD3D(void)
 {
     swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

     // close and release all existing COM objects
     pLayout->Release();
     pVS->Release();
     pPS->Release();
     pVBuffer->Release();
     swapchain->Release();
     backbuffer->Release();
     dev->Release();
     devcon->Release();
 }


 // this is the function that creates the shape to render
 void InitGraphics()
 {
     // create a triangle using the VERTEX struct
 	Vertex triangleVertices[] =
 	{
 		{{-0.25f, -0.25f * 1, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
 		{{-0.25f, 0.25f * 1, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
 		{{0.25f, 0.25f * 1, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},

 		{{-0.25f, -0.25f * 1, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
 		{{0.25f, -0.25f * 1, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
 		{{0.25f, 0.25f * 1, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}
 	};


     // create the vertex buffer
     D3D11_BUFFER_DESC bd;
     ZeroMemory(&bd, sizeof(bd));

     bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
     bd.ByteWidth = sizeof(Vertex) * POINTS_NUM;             // size is the VERTEX struct * 3
     bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
     bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

     dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


     // copy the vertices into the buffer
     D3D11_MAPPED_SUBRESOURCE ms;
     devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
     memcpy(ms.pData, triangleVertices, sizeof(triangleVertices));                 // copy the data
     devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
 }


 // this function loads and prepares the shaders
 void InitPipeline()
 {
     // load and compile the two shaders
     HRESULT result;
     	HRSRC src;
     	HGLOBAL res;
     // Пиксельный шейдера
 	src = FindResource(Instance, MAKEINTRESOURCE(IDR_BYTECODE_PIXEL), _T("ShaderObject"));
 	res = LoadResource(Instance, src);

 	result = dev->CreatePixelShader(res, SizeofResource(Instance, src),
 	                                   NULL, &pPS);
 	assert(SUCCEEDED(result));
 	FreeResource(res);

 	// Аналогично для вершинного шейдера
 	src = FindResource(Instance, MAKEINTRESOURCE(IDR_BYTECODE_VERTEX), _T("ShaderObject"));
 	res = LoadResource(Instance, src);
 	result = dev->CreateVertexShader(res, SizeofResource(Instance, src),
 	                                    NULL, &pVS);
 	assert(SUCCEEDED(result));
     // set the shader objects
     devcon->VSSetShader(pVS, 0, 0);
     devcon->PSSetShader(pPS, 0, 0);

     // create the input layout object
     D3D11_INPUT_ELEMENT_DESC ied[] =
     {
         {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
         {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
     };

     // dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
     	result = dev->CreateInputLayout(
 		// Массив описаний аргументов и его длина
             ied, 2,
 		// Байткод и его длина
 		res, SizeofResource(Instance, src),
 		// Структура ввода
             &pLayout);
 	assert(SUCCEEDED(result));
     devcon->IASetInputLayout(pLayout);
 }

 #endif Second
