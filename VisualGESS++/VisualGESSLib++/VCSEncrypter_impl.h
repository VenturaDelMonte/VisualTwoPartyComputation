/*
	file name: VCSEncrypter_impl.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once

#define TEMPLATE_DEF		template <class ImageType, u16 MAX_INPUT>
#define CSVCSEncrypter		CVCSEncrypter<ImageType, MAX_INPUT>

extern u32 g_start_time;

TEMPLATE_DEF
CSVCSEncrypter::CVCSEncrypter(IColorList* colors, u16 k, u16 n, u16 m, u16 h, u16 v) 
{
	u32 val = u32(GetQueryPerformaceCounter() & u32(-1));
	SetSeed(val);
	m_colors = colors;
	K = k; N = n; M = m; H = h; V = v;
	m_colors->Generate(n, m);
	m_shuffled = MemHandler.Alloc<int>(M);
	for (int i = 0; i < M; i++)
		m_shuffled[i] = i;
//	m_permuted_matrices.reserve(K);
	for (int k = 0; k < K; k++)
		m_permuted_matrices[(*m_colors)[k].first.Get()] = (*m_colors)[k].second;
#if 0
	Log("Start matrix");
	for (MATRICES_STORAGE_IT it = m_permuted_matrices.begin(), last = m_permuted_matrices.end(); it != last; ++it)
	{
		IMatrix* matrix = it->second;
		Log(">> 0x%x", it->first);
		string str = "";
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				str += format("0x%x ", matrix->getAt(i, j));
			}
			str += format("\n");
		}
		str += format("\n");
		Log("%s", str.c_str());
	}
#endif
}

TEMPLATE_DEF
CSVCSEncrypter::~CVCSEncrypter()
{
	//DeleteVector(m_input);
	DeleteVector(m_permuted_matrices);
	Delete(m_shuffled);
}


TEMPLATE_DEF
void CSVCSEncrypter::Encrypt(IMAGE_STORAGE& st, IMAGE_STORAGE& res)
{
	m_input = st;
	//ASSERT(m_input.size() == MAX_INPUT, "not enough or too much input images");
	Encrypt(res);
}

// reverese eng dalla jvm x setRegion...
// http://kickjava.com/src/java/awt/image/BufferedImage.java.htm

TEMPLATE_DEF 
void CSVCSEncrypter::Encrypt(IMAGE_STORAGE& res)
{
	ASSERT(m_input.size() == MAX_INPUT, "not enough or too much input images");
	CImageType* img = m_input[0];
	const u32 width = img->getWidth(), height = img->getHeight();
	for (int i = 1, new_width = width * H, new_height = height * V; i <= K; ++i)
		res.push_back(New<ImageType>(new_width, new_height)); 
		
	for (u32 j = 0; j < height; ++j)
	{
		for (u32 i = 0; i < width; ++i)
		{
			CImageType::ELEMENT_TYPE pixel = img->Get(i, j);
			ASSERT(m_colors->isAllowed(pixel), "invalid color");
			IMatrix* m = NextPermutation(pixel);
			u32 x = i * H, y = j * V; // pixel expansion
			for (u16 h = 0; h < res.size(); h++)
			{
				res[h]->setRegion(x, y, H, V, m->getAt(h), H);
			}
			Delete(m);
        }
	}
}

TEMPLATE_DEF
IMatrix* CSVCSEncrypter::NextPermutation(u32 clr)
{
	IMatrix* ret = New<IMatrix>(N, M);

	Shuffle(*this, m_shuffled, M);

	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < M; j++)
		{
			ASSERT(m_colors->isAllowed(clr), "wrong color");
			ret->setAt(i, j, m_permuted_matrices[clr]->getAt(i, m_shuffled[j]));
		}
	}
	return ret;
}
/*
TEMPLATE_DEF
IMatrix* CSVCSEncrypter::NextPermutation(bool clr)
{
	return NextPermutation(clr ? WHITE.Get() : BLACK.Get());
}
*/
#undef TEMPLATE_DEF
#undef CSVCSEncrypter


