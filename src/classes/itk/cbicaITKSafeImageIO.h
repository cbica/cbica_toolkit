/**
\file  cbicaITKSafeImageIO.h

\brief Defines safe input and output of itk::Images

Read and Write itk::Image data in a safe manner. Header-only

http://www.med.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2016 University of Pennsylvania. All rights reserved. <br>
See COPYING file or http://www.med.upenn.edu/sbia/software/license.html

*/
#pragma once

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkCastImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkOrientImageFilter.h"

#include "cbicaUtilities.h"
#include "cbicaITKImageInfo.h"

typedef itk::Image< float, 3 > ImageTypeFloat3D;
typedef itk::Image<unsigned int, 3> MaskType;

namespace cbica
{
  /**
  \brief Get the itk::ImageFileReader from input file name. This is useful for scenarios where reader meta information is needed for later writing step(s).

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputFileName = parser.getParameterValue("inputImage");
  ExpectedImageType::Pointer inputImage_1 = GetImageReader< ExpectedImageType >(inputFileName)->GetOutput();
  ExpectedImageType::Pointer inputImage_2 = GetImageReader< ExpectedImageType >(inputFileName, ".nii.gz,.img")->GetOutput();
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  \param fName name of the image
  \param supportedExtensions Supported extensions, defaults to ".nii.gz,.nii"
  \return itk::ImageFileReader::Pointer templated over the same as requested by user
  */
  template <class TImageType = ImageTypeFloat3D >
  typename itk::ImageFileReader< TImageType >::Pointer GetImageReader(const std::string &fName, const std::string &supportedExtensions = ".nii.gz,.nii", const std::string &delimitor = ",")
  {
    //// check read access
    //if (((_access(fName.c_str(), 4)) == -1) || ((_access(fName.c_str(), 6)) == -1))
    //{
    //  ShowErrorMessage("You don't have read access in selected location. Please check.");
    //  exit(EXIT_FAILURE);
    //}

    if (supportedExtensions != "")
    {
      std::vector< std::string > extensions = cbica::stringSplit(supportedExtensions, delimitor);

      bool supportedExtensionFound = false;
      for (size_t i = 0; i < extensions.size(); i++)
      {
        if (extensions[i] == cbica::getFilenameExtension(fName))
        {
          supportedExtensionFound = true;
        }
      }

      if (!supportedExtensionFound)
      {
        std::cerr << "Supplied file name '" << fName << "' doesn't have a supported extension. \nSupported Extensions: " << supportedExtensions << "\n";
        exit(EXIT_FAILURE);
      }
    }

    // ensure that the requested image dimensions and read image dimensions match up
    auto imageInfo = cbica::ImageInfo(fName);

    // perform basic sanity check
    if (imageInfo.GetImageDimensions() != TImageType::ImageDimension)
    {
      std::cerr << "Image Dimension mismatch. Return image is expected to be '" << TImageType::ImageDimension <<
        "'D and doesn't match the image dimension read from the input file, which is '" << imageInfo.GetImageDimensions() << "'.\n";
      exit(EXIT_FAILURE);
    }

    typedef itk::ImageFileReader< TImageType > ImageReaderType;
    typename ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName(fName);

    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject& e)
    {
      std::cerr << "Exception caught while reading the image '" << fName << "': " << e.what() << "\n";
      exit(EXIT_FAILURE);
    }

    return reader;
  }

  /**
  \brief Get the Dicom image reader (not the image, the READER). This is useful for scenarios where reader meta information is needed for later writing step(s).

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputDirName = parser.getParameterValue("inputDirName");
  ExpectedImageType::Pointer inputImage = GetDicomImageReader< ExpectedImageType >(inputDirName)->GetOutput(); // reads MRI and perfusion data by default tags "0008|0021,0020|0012"
  ExpectedImageType::Pointer inputImage = GetDicomImageReader< ExpectedImageType >(inputDirName, "0008|0021")->GetOutput(); // only reads images with tag "0008|0021"
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  \param dirName This is the directory name of the DICOM image which needs to be loaded - if this is an image, the underlying path of the image is considered
  */
  template <class TImageType = ImageTypeFloat3D >
  typename itk::ImageSeriesReader< TImageType >::Pointer GetDicomImageReader(const std::string &dirName)
  {
    std::string dirName_wrap = dirName;
    if (!cbica::isDir(dirName_wrap))
    {
      dirName_wrap = cbica::getFilenamePath(dirName);
    }
    dirName_wrap.pop_back(); // this is done to ensure the last "/" isn't taken into account for file name generation

    //// check read access
    //if (((_access(dirName_wrap.c_str(), 4)) == -1) || ((_access(dirName_wrap.c_str(), 6)) == -1))
    //{
    //  ShowErrorMessage("You don't have read access in selected location. Please check.");
    //  exit(EXIT_FAILURE);
    //}

    auto inputNames = itk::GDCMSeriesFileNames::New();
    inputNames->SetInputDirectory(dirName_wrap);
    //inputNames->SetLoadPrivateTags(true);
    using ReaderType = itk::ImageSeriesReader< TImageType >;
    typename ReaderType::Pointer seriesReader = ReaderType::New();
    //auto seriesReader = typename itk::ImageSeriesReader< TImageType >::New();

    auto dicomIO = itk::GDCMImageIO::New();
    seriesReader->SetImageIO(dicomIO);
    auto filenames = inputNames->GetInputFileNames();

    seriesReader->SetFileNames(filenames);
    try
    {
      seriesReader->Update();
    }
    catch (itk::ExceptionObject & err)
    {
      std::cerr << "Error while loading DICOM images: " << err.what() << "\n";
    }

    return seriesReader;
  }

  /**
  \brief Get the itk::Image from input dir name

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputDirName = parser.getParameterValue("inputDirName");
  ExpectedImageType::Pointer inputImage_1 = ReadDicomImage< ExpectedImageType >(inputFileName); // reads MRI and perfusion data by default tags "0008|0021,0020|0012"
  ExpectedImageType::Pointer inputImage_2 = ReadDicomImage< ExpectedImageType >(inputDirName, "0008|0021")->GetOutput(); // only reads images with tag "0008|0021"
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  \param fName name of the image
  \param supportedExtensions Supported extensions
  \return itk::ImageFileReader::Pointer templated over the same as requested by user
  */
  template <class TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer ReadDicomImage(const std::string &dirName)
  {
    return GetDicomImageReader< TImageType >(dirName)->GetOutput();
  }

  /**
  \brief Get the itk::Image from input dir name

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputDirName = parser.getParameterValue("inputDirName");
  ExpectedImageType::Pointer inputImage_1 = ReadDicomImage< ExpectedImageType >(inputFileName); // reads MRI and perfusion data by default tags "0008|0021,0020|0012"
  ExpectedImageType::Pointer inputImage_2 = ReadDicomImage< ExpectedImageType >(inputDirName, "0008|0021")->GetOutput(); // only reads images with tag "0008|0021"
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  This function calls ReadDicomImage<> internally

  \param fName name of the image
  \param supportedExtensions Supported extensions
  \return itk::ImageFileReader::Pointer templated over the same as requested by user
  */
  template <class TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer GetDicomImage(const std::string &dirName)
  {
    return ReadDicomImage< TImageType >(dirName);
  }


  /**
  \brief Write the itk::Image to the file name

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ComputedImageType;
  typedef itk::Image< unsigned char, 3 > WrittenImageType;
  ComputedImageType::Pointer imageToWrite = ComputedImageType::New();
  imageToWrite = GetImageSomehow();
  WriteImage< ComputedImageType >(imageToWrite, fileNameToWriteImage); // casts imageToWrite to WrittenImageType
  WriteImage< ComputedImageType, WrittenImageType >(imageToWrite, fileNameToWriteImage);  // writes imageToWrite as ComputedImageType
  // at this point, the image has already been written
  \endverbatim

  \param inputImage Pointer to processed image data which is to be written
  \param fileName File containing the image
  \return itk::Image of specified pixel and dimension type
  */
  template <typename ComputedImageType = ImageTypeFloat3D, typename ExpectedImageType = ComputedImageType>
  void WriteImage(typename ComputedImageType::Pointer imageToWrite, const std::string &fileName)
  {
    //// check write access
    //if (((_access(fileName.c_str(), 2)) == -1) || ((_access(fileName.c_str(), 6)) == -1))
    //{
    //  ShowErrorMessage("You don't have write access in selected location. Please check.");
    //  return;
    //}

    typedef itk::CastImageFilter<ComputedImageType, ExpectedImageType> CastFilterType;
    typename CastFilterType::Pointer filter = CastFilterType::New();
    filter->SetInput(imageToWrite);
    filter->Update();

    typedef typename itk::ImageFileWriter<ExpectedImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetInput(filter->GetOutput());
    writer->SetFileName(fileName);

    try
    {
      writer->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << "Error occurred while trying to write the image '" << fileName << "': " << e.what() << "\n";
      //exit(EXIT_FAILURE);//TBD all exit(EXIT_FAILURE) should be removed 
    }

    return;
  }

  /*
  \brief Write itk::ImageReader as DICOM to specified directory

  This uses default dictionary created by GDCM::ImageIO

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ComputedImageType;
  typedef itk::Image< unsigned char, 3 > WrittenImageType;
  itk::ImageSeriesReader< ComputedImageType >::Pointer inputImageReader = GetDicomImageReader< ComputedImageType >(inputDirName);
  ComputedImageType::Pointer imageToWrite = GetImageAfterProcessing( inputImageReader->GetOutput() );
  WriteImage< ComputedImageType, WrittenImageType >(imageToWrite, dirNameToWriteImage); // casts imageToWrite to WrittenImageType
  WriteImage< ComputedImageType >(imageToWrite, dirNameToWriteImage); // writes imageToWrite as ComputedImageType
  // at this point, the image has already been written
  \endverbatim

  \param imageToWrite Pointer to processed image data which is to be written
  \param dirName File containing the image
  \return itk::Image of specified pixel and dimension type
  */
  template <typename ComputedImageType, typename ExpectedImageType = ComputedImageType>
  void WriteDicomImage(const typename ComputedImageType::Pointer imageToWrite, const std::string &dirName)
  {
    auto reader = typename itk::ImageSeriesReader< ComputedImageType >::New();
    WriteDicomImage< ComputedImageType, ExpectedImageType >(reader, imageToWrite, dirName);
  }

  /**
  \brief Write the itk::Image as a DICOM to the specified directory

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ComputedImageType;
  typedef itk::Image< unsigned char, 3 > WrittenImageType;
  itk::ImageSeriesReader< ComputedImageType >::Pointer inputImageReader = GetDicomImageReader< ComputedImageType >(inputDirName);
  ComputedImageType::Pointer imageToWrite = GetImageAfterProcessing( inputImageReader->GetOutput() );
  WriteImage< ComputedImageType, WrittenImageType >(inputImageReader, imageToWrite, dirNameToWriteImage); // casts imageToWrite to WrittenImageType
  WriteImage< ComputedImageType >(inputImageReader, imageToWrite, dirNameToWriteImage); // writes imageToWrite as ComputedImageType
  // at this point, the image has already been written
  \endverbatim

  \param inputImageReader The image reader for DICOM - this is necessary to populate the DICOM dictionary properly
  \param imageToWrite Pointer to processed image data which is to be written
  \param dirName File containing the image
  \return itk::Image of specified pixel and dimension type
  */
  //using ComputedImageType = itk::Image< float, 3>;
  //using ExpectedImageType = itk::Image< float, 3>;
  template <typename ComputedImageType, typename ExpectedImageType = ComputedImageType>
  void WriteDicomImage(const typename itk::ImageSeriesReader< ComputedImageType >::Pointer inputImageReader, const typename ComputedImageType::Pointer imageToWrite, const std::string &dirName)
  {
    if (!cbica::isDir(dirName))
    {
      std::cout << "Specified directory wasn't found, creating...\n";
      cbica::createDir(dirName);
    }

    // check write access
    //if (((_access(dirName.c_str(), 2)) == -1) || ((_access(dirName.c_str(), 6)) == -1))
    //{
    //  ShowErrorMessage("You don't have write access in selected location. Please check.");
    //  return;
    //}

    typedef itk::CastImageFilter<ComputedImageType, ExpectedImageType> CastFilterType;
    typename CastFilterType::Pointer castFilter = CastFilterType::New();
    castFilter->SetInput(imageToWrite);
    castFilter->Update();

    //typedef typename ExpectedImageType::PixelType DicomPixelType;

    auto dicomIO = itk::GDCMImageIO::New();

    auto seriesWriter = itk::ImageSeriesWriter< ExpectedImageType, itk::Image<typename ExpectedImageType::PixelType, 2> >::New();

    auto namesGenerator = itk::NumericSeriesFileNames::New();
    //namesGenerator->SetUseSeriesDetails(false);
    auto start = imageToWrite->GetLargestPossibleRegion().GetIndex();
    auto size = imageToWrite->GetLargestPossibleRegion().GetSize();
    namesGenerator->SetSeriesFormat((dirName + "/image%03d.dcm").c_str());
    namesGenerator->SetStartIndex(start[2]);
    namesGenerator->SetEndIndex(start[2] + size[2] - 1);
    namesGenerator->SetIncrementIndex(1);

    seriesWriter->SetInput(castFilter->GetOutput());
    seriesWriter->SetImageIO(dicomIO);
    seriesWriter->SetFileNames(namesGenerator->GetFileNames());

    typename itk::ImageSeriesReader< ComputedImageType >::DictionaryArrayType outputArray;
    if (inputImageReader.IsNull() || (inputImageReader->GetImageIO() == NULL))
    {
      //dicomIO->SetOrigin(0, imageToWrite->GetOrigin()[0]);
      //dicomIO->SetOrigin(1, imageToWrite->GetOrigin()[1]);
      //dicomIO->SetOrigin(2, imageToWrite->GetOrigin()[2]);
      //dicomIO->SetSpacing(0, imageToWrite->GetSpacing()[0]);
      //dicomIO->SetSpacing(1, imageToWrite->GetSpacing()[1]);
      //dicomIO->SetSpacing(2, imageToWrite->GetSpacing()[2]);
      //dicomIO->SetDimensions(0, imageToWrite->GetLargestPossibleRegion().GetSize()[0]);
      //dicomIO->SetDimensions(1, imageToWrite->GetLargestPossibleRegion().GetSize()[1]);
      //dicomIO->SetDimensions(2, imageToWrite->GetLargestPossibleRegion().GetSize()[2]);

      typename ExpectedImageType::IndexType index;
      index[0] = 0;
      index[1] = 0;
      for (size_t i = 0; i < imageToWrite->GetLargestPossibleRegion().GetSize()[2]; i++)
      {
        auto dict = new typename itk::ImageSeriesReader< ComputedImageType >::DictionaryType;
        typename ExpectedImageType::PointType position;
        index[2] = i;
        imageToWrite->TransformIndexToPhysicalPoint(index, position);
        itk::EncapsulateMetaData<std::string>(*dict, "0020|0032", std::to_string(position[0]) + "\\" + std::to_string(position[1]) + "\\" + std::to_string(position[2])); // patient position
        itk::EncapsulateMetaData<std::string>(*dict, "0020|1041", std::to_string(position[0]) + "\\" + std::to_string(position[1]) + "\\" + std::to_string(position[2])); // slice location
        //itk::EncapsulateMetaData<std::string>(*dict, "0020|1041", "0\\0\\0"); // slice location
        //itk::EncapsulateMetaData<std::string>(*dict, "0020|0011", std::to_string(1)); 
        //itk::EncapsulateMetaData<std::string>(*dict, "0020|0013", std::to_string(i)); 
        //itk::EncapsulateMetaData<std::string>(*dict, "0018|5100", std::to_string(position[0]) + "\\" + std::to_string(position[1]) + "\\" + std::to_string(position[2]));
        //itk::EncapsulateMetaData<std::string>(*dict, "2020|0010", std::to_string(position[0]) + "\\" + std::to_string(position[1]) + "\\" + std::to_string(position[2]));
        //itk::EncapsulateMetaData<std::string>(*dict, "0018|5101", std::to_string(position[0]) + "\\" + std::to_string(position[1]) + "\\" + std::to_string(position[2]));
        //itk::EncapsulateMetaData<std::string>(*dict, "0020|0037", std::to_string(*imageToWrite->GetDirection()[0]) + "\\" + std::to_string(*imageToWrite->GetDirection()[1]) + "\\0\\" + std::to_string(*imageToWrite->GetDirection()[3]) + "\\" + std::to_string(*imageToWrite->GetDirection()[4]) + "\\0"); // orientation
        itk::EncapsulateMetaData<std::string>(*dict, "0018|0050", std::to_string(imageToWrite->GetSpacing()[2])); // Slice Thickness
        //itk::EncapsulateMetaData<std::string>(*dict, "0018|0088", std::to_string(imageToWrite->GetSpacing()[2])); // Spacing Between Slices
        //itk::EncapsulateMetaData<std::string>(*dict, "0008|0008", "DERIVED\\SECONDARY"); // Image Type
        //itk::EncapsulateMetaData<std::string>(*dict, "0008|0064", "DV"); // Conversion Type
        //itk::EncapsulateMetaData<std::string>(*dict, "0008|0060", "MR"); // Modality - can never gurantee MR
        //itk::EncapsulateMetaData<std::string>(*dict, "0018|0088", std::to_string(imageToWrite->GetSpacing()[2]));

        outputArray.push_back(dict);
      }

      seriesWriter->SetMetaDataDictionaryArray(&outputArray);
    }
    else
    {
      dicomIO->SetMetaDataDictionary(inputImageReader->GetMetaDataDictionary());
      seriesWriter->SetMetaDataDictionaryArray(inputImageReader->GetMetaDataDictionaryArray()); // no dictionary information present without seriesReader
    }

    try
    {
      seriesWriter->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << "Error occurred while trying to write the image '" << dirName << "': " << e.what() << "\n";
      exit(EXIT_FAILURE);
    }

  }


  /**
  \brief Get the itk::Image from input file name

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputFileName = parser.getParameterValue("inputImage");
  ExpectedImageType::Pointer inputImage_1 = ReadImage< ExpectedImageType >(inputFileName);
  ExpectedImageType::Pointer inputImage_2 = ReadImage< ExpectedImageType >(inputFileName, ".nii.gz,.img");
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  \param fName name of the image
  \param supportedExtensions Supported extensions, defaults to ".nii.gz,.nii"
  \return itk::ImageFileReader::Pointer templated over the same as requested by user
  */
  template <class TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer ReadImage(const std::string &fName, const std::string &supportedExtensions = ".nii.gz,.nii", const std::string &delimitor = ",")
  {
    std::string extension = cbica::getFilenameExtension(fName);
    if (cbica::isDir(fName) || (extension == ".dcm") || (extension == ".dicom"))
    {
      return GetDicomImage< TImageType >(fName);
    }
    else
    {
      return GetImageReader< TImageType >(fName, supportedExtensions, delimitor)->GetOutput();
    }
  }

  template< class TImageType >
  typename TImageType::Pointer ReadImageWithOrientFix(const std::string &fName, const std::string &supportedExtensions = ".nii.gz,.nii", const std::string &delimitor = ",")
  {
    std::string extension = cbica::getFilenameExtension(fName);
    if (cbica::isDir(fName) || (extension == ".dcm") || (extension == ".dicom"))
    {
      return GetDicomImage< TImageType >(fName);
    }
    else
    {
      typename TImageType::Pointer rval = GetImageReader< TImageType >(fName, supportedExtensions, delimitor)->GetOutput();
      typename itk::OrientImageFilter<TImageType, TImageType>::Pointer orienter = itk::OrientImageFilter<TImageType, TImageType>::New();
      orienter->UseImageDirectionOn();
      orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
      orienter->SetInput(rval);
      orienter->Update();
      rval = orienter->GetOutput();
      return rval;
    }
  }

  /**
  \brief Get the itk::Image from input file name

  Usage:
  \verbatim
  typedef itk::Image< float, 3 > ExpectedImageType;
  std::string inputFileName = parser.getParameterValue("inputImage");
  ExpectedImageType::Pointer inputImage_1 = ReadImage< ExpectedImageType >(inputFileName);
  ExpectedImageType::Pointer inputImage_2 = ReadImage< ExpectedImageType >(inputFileName, ".nii.gz,.img");
  DoAwesomeStuffWithImage( inputImage );
  \endverbatim

  This function calls Same as ReadImage internally
  \param fName name of the image
  \param supportedExtensions Supported extensions, defaults to ".nii.gz,.nii"
  \return itk::ImageFileReader::Pointer templated over the same as requested by user
  */
  template <class TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer GetImage(const std::string &fName, const std::string &supportedExtensions = ".nii.gz,.nii", const std::string &delimitor = ",")
  {
    return ReadImage< TImageType >(fName, supportedExtensions, delimitor);
  }

}
