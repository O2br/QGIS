/***************************************************************************
                             qgslayoutatlas.h
                             ----------------
    begin                : December 2017
    copyright            : (C) 2017 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSLAYOUTATLAS_H
#define QGSLAYOUTATLAS_H

#include "qgis_core.h"
#include "qgsvectorlayerref.h"
#include "qgslayoutserializableobject.h"
#include "qgsabstractlayoutiterator.h"
#include <QObject>

class QgsLayout;

/**
 * \ingroup core
 * Class used to render an Atlas, iterating over geometry features.
 * prepareForFeature() modifies the atlas map's extent to zoom on the given feature.
 * This class is used for printing, exporting to PDF and images.
 * \note This class should not be created directly. For the atlas to function correctly
 * the atlasComposition() property for QgsComposition should be used to retrieve a
 * QgsLayoutAtlas which is automatically created and attached to the composition.
 * \since QGIS 3.0
 */
class CORE_EXPORT QgsLayoutAtlas : public QObject, public QgsAbstractLayoutIterator, public QgsLayoutSerializableObject
{
    Q_OBJECT
  public:

    /**
     * Constructor for new QgsLayoutAtlas.
     */
    QgsLayoutAtlas( QgsLayout *layout SIP_TRANSFERTHIS );

    QString stringType() const override;
    QgsLayout *layout() override;

    /**
     * Returns the atlas' layout.
     * \note Not available in Python bindings.
     */
    const QgsLayout *layout() const SIP_SKIP;

    bool writeXml( QDomElement &parentElement, QDomDocument &document, const QgsReadWriteContext &context ) const override;
    bool readXml( const QDomElement &element, const QDomDocument &document, const QgsReadWriteContext &context ) override;

    /**
     * Returns whether the atlas generation is enabled
     * \see setEnabled()
     */
    bool enabled() const { return mEnabled; }

    /**
     * Sets whether the atlas is \a enabled.
     * \see enabled()
     */
    void setEnabled( bool enabled );

    /**
     * Returns true if the atlas is set to hide the coverage layer.
     * \see setHideCoverage()
     */
    bool hideCoverage() const { return mHideCoverage; }

    /**
     * Sets whether the coverage layer should be hidden in map items in the layouts.
     * \see hideCoverage()
     */
    void setHideCoverage( bool hide );

    /**
     * Returns the filename expression used for generating output filenames for each
     * atlas page.
     * \see setFilenameExpression()
     * \see currentFilename()
     */
    QString filenameExpression() const { return mFilenameExpressionString; }

    /**
     * Sets the filename \a expression used for generating output filenames for each
     * atlas page.
     * If an invalid expression is passed, false will be returned and \a errorString
     * will be set to the expression error.
     * \see filenameExpression()
     * \see currentFilename()
     */
    bool setFilenameExpression( const QString &expression, QString &errorString SIP_OUT );

    /**
     * Returns the current feature filename.
     * \see filenameExpression()
     * \see setFilenameExpression()
     */
    QString currentFilename() const;

    /**
     * Returns the coverage layer used for the atlas features.
     * \see setCoverageLayer()
     */
    QgsVectorLayer *coverageLayer() const { return mCoverageLayer.get(); }

    /**
     * Sets the coverage \a layer to use for the atlas features.
     * \see coverageLayer()
     */
    void setCoverageLayer( QgsVectorLayer *layer );

    /**
     * Returns the expression (or field name) used for calculating the page name.
     * \see setPageNameExpression()
     * \see nameForPage()
     */
    QString pageNameExpression() const { return mPageNameExpression; }

    /**
     * Sets the \a expression (or field name) used for calculating the page name.
     * \see pageNameExpression()
     */
    void setPageNameExpression( const QString &expression ) { mPageNameExpression = expression; }

    /**
     * Returns the calculated name for a specified atlas \a page number. Page numbers start at 0.
     * \see pageNameExpression()
     */
    QString nameForPage( int page ) const;

    /**
     * Returns true if features should be sorted in the atlas.
     * \see setSortFeatures()
     * \see sortAscending()
     * \see sortExpression()
     */
    bool sortFeatures() const { return mSortFeatures; }

    /**
     * Sets whether features should be sorted in the atlas.
     * \see sortFeatures()
     * \see setSortAscending()
     * \see setSortExpression()
     */
    void setSortFeatures( bool enabled ) { mSortFeatures = enabled; }

    /**
     * Returns true if features should be sorted in an ascending order.
     *
     * This property has no effect is sortFeatures() is false.
     *
     * \see sortFeatures()
     * \see setSortAscending()
     * \see sortExpression()
     */
    bool sortAscending() const { return mSortAscending; }

    /**
     * Sets whether features should be sorted in an ascending order.
     *
     * This property has no effect is sortFeatures() is false.
     *
     * \see setSortFeatures()
     * \see sortAscending()
     * \see setSortExpression()
     */
    void setSortAscending( bool ascending ) { mSortAscending = ascending; }

    /**
     * Returns the expression (or field name) to use for sorting features.
     *
     * This property has no effect is sortFeatures() is false.
     *
     * \see sortFeatures()
     * \see sortAscending()
     * \see setSortExpression()
     */
    QString sortExpression() const { return mSortExpression; }

    /**
     * Sets the \a expression (or field name) to use for sorting features.
     *
     * This property has no effect is sortFeatures() is false.
     *
     * \see setSortFeatures()
     * \see setSortAscending()
     * \see sortExpression()
     */
    void setSortExpression( const QString &expression ) { mSortExpression = expression; }

    /**
     * Returns true if features should be filtered in the coverage layer.
     * \see filterExpression()
     * \see setFilterExpression()
     */
    bool filterFeatures() const { return mFilterFeatures; }

    /**
     * Sets whether features should be \a filtered in the coverage layer.
     * \see filterFeatures()
     * \see setFilterExpression()
     */
    void setFilterFeatures( bool filtered ) { mFilterFeatures = filtered; }

    /**
     * Returns the expression used for filtering features in the coverage layer.
     *
     * This property has no effect is filterFeatures() is false.
     *
     * \see setFilterExpression()
     * \see filterFeatures()
     */
    QString filterExpression() const { return mFilterExpression; }

    /**
     * Sets the \a expression used for filtering features in the coverage layer.
     *
     * This property has no effect is filterFeatures() is false.
     *
     * If an invalid expression is passed, false will be returned and \a errorString
     * will be set to the expression error.
     *
     * \see filterExpression()
     * \see setFilterFeatures()
     */
    bool setFilterExpression( const QString &expression, QString &errorString SIP_OUT );

    /**
     * Requeries the current atlas coverage layer and applies filtering and sorting. Returns
     * number of matching features.
     */
    int updateFeatures();

    bool beginRender() override;
    bool endRender() override;
    int count() override;
    QString filePath( const QString &baseFilePath, const QString &extension ) override;

    /**
     * Returns the current feature number, where a value of 0 corresponds to the first feature.
     */
    int currentFeatureNumber() const { return mCurrentFeatureNo; }

  public slots:

    bool next() override;

    /**
     * Iterates to the previous feature, returning false if no previous feature exists.
     * \see next()
     * \see last()
     * \see first()
     * \see seekTo()
     */
    bool previous();

    /**
     * Seeks to the last feature, returning false if no feature was found.
     * \see next()
     * \see previous()
     * \see first()
     * \see seekTo()
     */
    bool last();

    /**
     * Seeks to the first feature, returning false if no feature was found.
     * \see next()
     * \see previous()
     * \see last()
     * \see seekTo()
     */
    bool first();

    /**
     * Seeks to the specified \a feature number.
     * \see first()
     * \see previous()
     * \see next()
     * \see last()
     */
    bool seekTo( int feature );

    /**
     * Seeks to the specified \a feature.
     * \see first()
     * \see previous()
     * \see next()
     * \see last()
     */
    bool seekTo( const QgsFeature &feature );

    /**
     * Refreshes the current atlas feature, by refetching its attributes from the vector layer provider
     */
    void refreshCurrentFeature();

  signals:

    //! Emitted when one of the atlas parameters changes.
    void changed();

    //! Emitted when atlas is enabled or disabled.
    void toggled( bool );

    //! Emitted when the coverage layer for the atlas changes.
    void coverageLayerChanged( QgsVectorLayer *layer );

    //! Is emitted when the atlas has an updated status bar \a message.
    void messagePushed( const QString &message );

    /**
     * Emitted when the number of features for the atlas changes.
     */
    void numberFeaturesChanged( int numFeatures );

    //! Is emitted when the current atlas \a feature changes.
    void featureChanged( const QgsFeature &feature );

    //! Emitted when atlas rendering has begun.
    void renderBegun();

    //! Emitted when atlas rendering has ended.
    void renderEnded();

  private slots:
    void removeLayers( const QStringList &layers );

  private:

    /**
     * Updates the filename expression.
     * \returns true if expression was successfully parsed, false if expression is invalid
     */
    bool updateFilenameExpression( QString &error );

    /**
     * Evaluates filename for current feature
     * \returns true if feature filename was successfully evaluated
     */
    bool evalFeatureFilename( const QgsExpressionContext &context );

    /**
     * Prepare the atlas for the given feature. Sets the extent and context variables
     * \param i feature number
     * \returns true if feature was successfully prepared
     */
    bool prepareForFeature( int i );

    QPointer< QgsLayout > mLayout;

    bool mEnabled = false;
    bool mHideCoverage = false;
    QString mFilenameExpressionString;

    QgsExpression mFilenameExpression;
    QgsVectorLayerRef mCoverageLayer;

    QString mCurrentFilename;
    bool mSortFeatures = false;
    bool mSortAscending = true;

    typedef QMap< QgsFeatureId, QVariant > SorterKeys;
    // value of field that is used for ordering of features
    SorterKeys mFeatureKeys;

    QString mSortExpression;

    QString mPageNameExpression;

    bool mFilterFeatures = false;
    QString mFilterExpression;

    QString mFilterParserError;

    // id of each iterated feature (after filtering and sorting) paired with atlas page name
    QVector< QPair<QgsFeatureId, QString> > mFeatureIds;
    // current atlas feature number
    int mCurrentFeatureNo = -1;
    QgsFeature mCurrentFeature;

    QgsExpressionContext createExpressionContext();

    friend class AtlasFeatureSorter;
};

#endif //QGSLAYOUTATLAS_H



